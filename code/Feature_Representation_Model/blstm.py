"""
blstm.py -- BiLSTM feature representation model of CDVD-DPW (paper 3.3.3)

Architecture exactly as described in the paper:

    Input  (500 tokens x 50-d embeddings)
      -> Bidirectional LSTM, hidden size h = 64 (return_sequences=True)
      -> Global max pooling over the concatenated bidirectional states
         (dimension 2h = 128)
      -> feature vector z in R^128
      -> Dense softmax layer (2 classes)

Supervised pre-training with cross-entropy loss prevents feature collapse
and ensures discriminative representations for cross-domain transfer.
The 128-d pooled activations are the feature vectors handed to the
Dual-Stage Label Propagation and CDR modules.
"""

from __future__ import print_function

import os
import warnings

warnings.filterwarnings("ignore")

import numpy as np
import tensorflow as tf
from tensorflow.keras.layers import (LSTM, Bidirectional, Dense,
                                     GlobalMaxPooling1D, Input)
from tensorflow.keras.models import Model
from tensorflow.keras.optimizers import Adamax
from tensorflow.keras.utils import to_categorical
from sklearn.metrics import classification_report, confusion_matrix

HIDDEN = 64          # LSTM hidden size h; bidirectional gives 2h = 128
FEATURE_DIM = 2 * HIDDEN


class BLSTM:
    def __init__(self, data, name="", batch_size=64, epochs=100,
                 enable_undersampling=False, seed=42):
        """
        data: DataFrame with columns 'vector' (500x50 ndarray) and 'val' (0/1)
        """
        np.random.seed(seed)
        tf.random.set_seed(seed)
        for device in tf.config.list_physical_devices("GPU"):
            try:
                tf.config.experimental.set_memory_growth(device, True)
            except RuntimeError:
                pass
        vectors = np.stack(data["vector"].values)
        labels = data["val"].values.astype(int)

        # Validation is selected at complete-function granularity.  This
        # validation score is diagnostic only; it is not used for checkpoint
        # selection, but grouping prevents optimistic slice overlap.
        groups = (data["function_id"].astype(str).values
                  if "function_id" in data else np.arange(len(data)).astype(str))
        rng = np.random.default_rng(seed)
        val_groups = set()
        group_labels = {}
        for group, label in zip(groups, labels):
            group_labels.setdefault(group, set()).add(int(label))
        for label in (0, 1):
            candidates = sorted(g for g, labs in group_labels.items()
                                if labs == {label})
            if not candidates:
                continue
            n_val = max(1, int(round(0.1 * len(candidates))))
            if len(candidates) > 1:
                n_val = min(n_val, len(candidates) - 1)
            val_groups.update(rng.choice(candidates, n_val,
                                         replace=False).tolist())
        val_mask = np.array([g in val_groups for g in groups])
        train_idx = np.where(~val_mask)[0]
        val_idx = np.where(val_mask)[0]

        # Optional class balancing is applied only after the grouped split.
        if enable_undersampling:
            positive = train_idx[labels[train_idx] == 1]
            negative = train_idx[labels[train_idx] == 0]
            n_keep = min(len(positive), len(negative))
            if n_keep:
                negative = rng.choice(negative, n_keep, replace=False)
                positive = rng.choice(positive, n_keep, replace=False)
                train_idx = np.concatenate([positive, negative])
                rng.shuffle(train_idx)

        X_train, X_val = vectors[train_idx], vectors[val_idx]
        y_train, y_val = labels[train_idx], labels[val_idx]
        self.X_train, self.X_val = X_train, X_val
        self.y_train = to_categorical(y_train, 2)
        self.y_val = to_categorical(y_val, 2)
        self.train_function_ids = sorted(set(groups[train_idx]))
        self.val_function_ids = sorted(set(groups[val_idx]))

        self.name = name
        self.dir = ""
        self.batch_size = batch_size
        self.epochs = epochs

        # ---- model (paper 3.3.3) ----
        inputs = Input(shape=(vectors.shape[1], vectors.shape[2]))
        h = Bidirectional(LSTM(HIDDEN, return_sequences=True))(inputs)
        z = GlobalMaxPooling1D(name="feature")(h)      # z in R^128
        outputs = Dense(2, activation="softmax")(z)
        model = Model(inputs, outputs)
        model.compile(Adamax(learning_rate=0.002),
                      "categorical_crossentropy", metrics=["accuracy"])
        model.summary()
        self.model = model
        self.feature_model = Model(inputs, z)

    # ------------------------------------------------------------------ #
    def weights_path(self):
        return os.path.join(self.dir, self.name + "_model.weights.h5")

    def train(self):
        """Supervised pre-training with cross-entropy loss."""
        self.model.fit(self.X_train, self.y_train,
                       batch_size=self.batch_size,
                       validation_data=(self.X_val, self.y_val),
                       epochs=self.epochs)
        self.model.save_weights(self.weights_path())

    def test(self):
        if os.path.exists(self.weights_path()):
            self.model.load_weights(self.weights_path())
        predictions = self.model.predict(
            self.X_val, batch_size=self.batch_size)
        y_true = np.argmax(self.y_val, axis=1)
        y_pred = np.argmax(predictions, axis=1)
        tn, fp, fn, tp = confusion_matrix(y_true, y_pred).ravel()
        recall = tp / max(tp + fn, 1)
        precision = tp / max(tp + fp, 1)
        f1 = 2 * precision * recall / max(precision + recall, 1e-12)
        print("Precision: %.4f  Recall: %.4f  F1: %.4f" %
              (precision, recall, f1))
        print(classification_report(y_true, y_pred))

    # ------------------------------------------------------------------ #
    def extract_features(self, data):
        """Return (features [n x 128], labels [n]) for a vector DataFrame."""
        vectors = np.stack(data["vector"].values)
        labels = data["val"].values.astype(int)
        features = self.feature_model.predict(vectors,
                                              batch_size=self.batch_size)
        return features, labels

    def obtain(self, data, name):
        """Extract 128-d features and save '<name>_labels_features_ext.csv'."""
        features, labels = self.extract_features(data)
        data_with_labels = np.hstack((features, labels.reshape(-1, 1)))
        output_path = os.path.join(self.dir, name + "_labels_features_ext.csv")
        fmt = ["%.6f"] * features.shape[1] + ["%d"]
        np.savetxt(output_path, data_with_labels, delimiter=",", fmt=fmt)
        print("Features and labels saved to", output_path)
        return output_path
