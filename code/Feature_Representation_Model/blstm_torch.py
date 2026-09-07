"""PyTorch 1.12-compatible BiLSTM representation backend.

Architecture: 50-d token embeddings -> BiLSTM(64 per direction) -> global
max pooling -> 128-d z -> two-class linear head.  The pooled vector is the
feature consumed by DLP and DFW.
"""

from __future__ import print_function

import os

import numpy as np
import torch
from sklearn.metrics import classification_report, confusion_matrix
from torch import nn
from torch.utils.data import DataLoader, TensorDataset

HIDDEN = 64


class _Encoder(nn.Module):
    def __init__(self, input_dim):
        super().__init__()
        self.lstm = nn.LSTM(input_dim, HIDDEN, batch_first=True,
                            bidirectional=True)
        self.classifier = nn.Linear(2 * HIDDEN, 2)

    def forward(self, inputs, features_only=False):
        states, _ = self.lstm(inputs)
        features = torch.max(states, dim=1).values
        if features_only:
            return features
        return self.classifier(features)


def _grouped_indices(data, labels, seed):
    groups = (data["function_id"].astype(str).values
              if "function_id" in data else np.arange(len(data)).astype(str))
    rng = np.random.default_rng(seed)
    group_labels = {}
    for group, label in zip(groups, labels):
        group_labels.setdefault(group, set()).add(int(label))
    validation = set()
    for label in (0, 1):
        candidates = sorted(g for g, labs in group_labels.items()
                            if labs == {label})
        if len(candidates) <= 1:
            continue
        n_val = min(max(1, int(round(0.1 * len(candidates)))),
                    len(candidates) - 1)
        validation.update(rng.choice(candidates, n_val,
                                     replace=False).tolist())
    val_mask = np.array([g in validation for g in groups])
    return np.where(~val_mask)[0], np.where(val_mask)[0], groups


class BLSTMTorch:
    def __init__(self, data, name="", batch_size=64, epochs=100,
                 enable_undersampling=False, seed=42):
        np.random.seed(seed)
        torch.manual_seed(seed)
        if torch.cuda.is_available():
            torch.cuda.manual_seed_all(seed)
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.vectors = np.stack(data["vector"].values).astype(np.float32)
        self.labels = data["val"].values.astype(np.int64)
        train_idx, val_idx, groups = _grouped_indices(data, self.labels, seed)
        rng = np.random.default_rng(seed)
        if enable_undersampling:
            positive = train_idx[self.labels[train_idx] == 1]
            negative = train_idx[self.labels[train_idx] == 0]
            n_keep = min(len(positive), len(negative))
            if n_keep:
                train_idx = np.concatenate([
                    rng.choice(positive, n_keep, replace=False),
                    rng.choice(negative, n_keep, replace=False)])
                rng.shuffle(train_idx)
        self.train_idx, self.val_idx = train_idx, val_idx
        self.train_function_ids = sorted(set(groups[train_idx]))
        self.val_function_ids = sorted(set(groups[val_idx]))
        self.name = name
        self.dir = ""
        self.batch_size = batch_size
        self.epochs = epochs
        self.model = _Encoder(self.vectors.shape[2]).to(self.device)

    def weights_path(self):
        return os.path.join(self.dir, self.name + "_model.pt")

    def _loader(self, indices, shuffle=False):
        dataset = TensorDataset(torch.from_numpy(self.vectors[indices]),
                                torch.from_numpy(self.labels[indices]))
        generator = torch.Generator().manual_seed(42)
        return DataLoader(dataset, batch_size=self.batch_size, shuffle=shuffle,
                          generator=generator)

    def train(self):
        optimizer = torch.optim.Adamax(self.model.parameters(), lr=0.002)
        criterion = nn.CrossEntropyLoss()
        for epoch in range(self.epochs):
            self.model.train()
            total_loss = 0.0
            for vectors, labels in self._loader(self.train_idx, shuffle=True):
                vectors, labels = vectors.to(self.device), labels.to(self.device)
                optimizer.zero_grad()
                loss = criterion(self.model(vectors), labels)
                loss.backward()
                optimizer.step()
                total_loss += loss.item() * len(labels)
            print("epoch %03d/%03d loss %.6f" %
                  (epoch + 1, self.epochs, total_loss / max(len(self.train_idx), 1)))
        torch.save(self.model.state_dict(), self.weights_path())

    def test(self):
        if not len(self.val_idx):
            print("validation skipped: no disjoint function group available")
            return
        truth = self.labels[self.val_idx]
        pred = self._predict_indices(self.val_idx)
        tn, fp, fn, tp = confusion_matrix(truth, pred, labels=[0, 1]).ravel()
        recall = tp / max(tp + fn, 1)
        precision = tp / max(tp + fp, 1)
        f1 = 2 * precision * recall / max(precision + recall, 1e-12)
        print("Precision: %.4f  Recall: %.4f  F1: %.4f" %
              (precision, recall, f1))
        print(classification_report(truth, pred, zero_division=0))

    def _predict_indices(self, indices):
        self.model.eval()
        predictions = []
        with torch.no_grad():
            for vectors, _ in self._loader(indices):
                logits = self.model(vectors.to(self.device))
                predictions.extend(logits.argmax(1).cpu().numpy().tolist())
        return np.asarray(predictions)

    def extract_features(self, data):
        vectors = np.stack(data["vector"].values).astype(np.float32)
        labels = data["val"].values.astype(int)
        loader = DataLoader(TensorDataset(torch.from_numpy(vectors)),
                            batch_size=self.batch_size)
        features = []
        self.model.eval()
        with torch.no_grad():
            for (batch,) in loader:
                z = self.model(batch.to(self.device), features_only=True)
                features.append(z.cpu().numpy())
        return np.concatenate(features, axis=0), labels

    def obtain(self, data, name):
        features, labels = self.extract_features(data)
        output_path = os.path.join(self.dir, name + "_labels_features_ext.csv")
        values = np.hstack([features, labels.reshape(-1, 1)])
        fmt = ["%.6f"] * features.shape[1] + ["%d"]
        np.savetxt(output_path, values, delimiter=",", fmt=fmt)
        return output_path

