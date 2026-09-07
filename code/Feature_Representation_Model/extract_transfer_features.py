#!/usr/bin/env python3
"""Build source and target features in one shared 128-dimensional space.

This is the paper-faithful entry point for a directed transfer task.  Unlike
the historical single-domain driver, it trains one Word2Vec vocabulary and one
BiLSTM encoder, performs exact model-input deduplication before encoding,
removes target-side cross-domain duplicates, and allocates label budgets by
complete C function.

Example:
  python extract_transfer_features.py source.cgd target.cgd output/run01 \
      --source-functions ../LibPNG --target-functions ../LibTIFF
"""

import argparse
import json
import os

import numpy as np
import pandas as pd
import scipy.io

from slice_dataset import (consolidate_model_inputs, function_group_split,
                           parse_cgd, records_to_frame,
                           remove_cross_domain_duplicates, train_vectorizer,
                           write_manifest)


def save_mat(path, features, labels):
    data = np.hstack((features, labels.reshape(-1, 1).astype(float)))
    scipy.io.savemat(path, {"data": data})
    print("Saved", path, "shape", data.shape)


def save_split(model, records, path, vectorizer):
    frame = records_to_frame(records, vectorizer)
    features, labels = model.extract_features(frame)
    save_mat(path, features, labels)
    return frame


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source_cgd")
    parser.add_argument("target_cgd")
    parser.add_argument("output_dir")
    parser.add_argument("--source-functions", required=True,
                        help="original complete-function tree for exact deduplication")
    parser.add_argument("--target-functions", required=True,
                        help="original complete-function tree for exact deduplication")
    parser.add_argument("--source-label-fraction", type=float, default=0.30)
    parser.add_argument("--target-label-fraction", type=float, default=0.05)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--epochs", type=int, default=100)
    parser.add_argument("--batch-size", type=int, default=64)
    parser.add_argument("--undersample", action="store_true",
                        help="balance encoder training after grouped validation split")
    parser.add_argument("--backend", choices=("torch", "tensorflow"),
                        default="torch",
                        help="paper backend is torch; tensorflow is retained for compatibility")
    args = parser.parse_args()

    os.makedirs(args.output_dir, exist_ok=True)
    source_dir = os.path.join(args.output_dir, "source")
    target_dir = os.path.join(args.output_dir, "target")
    os.makedirs(source_dir, exist_ok=True)
    os.makedirs(target_dir, exist_ok=True)

    source_raw = parse_cgd(args.source_cgd)
    target_raw = parse_cgd(args.target_cgd)
    source, source_manifest = consolidate_model_inputs(source_raw)
    target, target_manifest = consolidate_model_inputs(target_raw)
    target, cross_removed = remove_cross_domain_duplicates(
        source, target, args.source_functions, args.target_functions)

    source_l, source_u = function_group_split(
        source, args.source_label_fraction, args.seed)
    target_l, target_u = function_group_split(
        target, args.target_label_fraction, args.seed + 1)

    # Vocabulary learning is unsupervised.  All source and target inputs may
    # define token embeddings, but only L_s and L_t contribute labels to the
    # supervised BiLSTM objective.
    vectorizer = train_vectorizer([source, target], seed=args.seed)
    source_l_df = records_to_frame(source_l, vectorizer)
    target_l_df = records_to_frame(target_l, vectorizer)
    source_l_df["function_id"] = "source::" + source_l_df["function_id"].astype(str)
    target_l_df["function_id"] = "target::" + target_l_df["function_id"].astype(str)
    train_df = pd.concat([source_l_df, target_l_df], ignore_index=True)

    if args.backend == "torch":
        from blstm_torch import BLSTMTorch
        model_class = BLSTMTorch
    else:
        from blstm import BLSTM
        model_class = BLSTM
    model = model_class(train_df, name="shared_encoder",
                        batch_size=args.batch_size, epochs=args.epochs,
                        enable_undersampling=args.undersample, seed=args.seed)
    model.dir = args.output_dir
    model.train()
    model.test()

    source_pct = int(round(100 * args.source_label_fraction))
    target_pct = int(round(100 * args.target_label_fraction))
    save_split(model, source_l,
               os.path.join(source_dir, "%d%%_labels_features_ext.mat" % source_pct),
               vectorizer)
    save_split(model, source_u,
               os.path.join(source_dir, "%d%%_labels_features_ext.mat" % (100-source_pct)),
               vectorizer)
    save_split(model, target_l,
               os.path.join(target_dir, "%d%%_labels_features_ext.mat" % target_pct),
               vectorizer)
    save_split(model, target_u,
               os.path.join(target_dir, "%d%%_labels_features_ext.mat" % (100-target_pct)),
               vectorizer)

    write_manifest(os.path.join(args.output_dir, "source_slice_manifest.csv"),
                   source_manifest)
    write_manifest(os.path.join(args.output_dir, "target_slice_manifest.csv"),
                   target_manifest)
    write_manifest(os.path.join(args.output_dir, "cross_domain_removed.csv"),
                   cross_removed)
    source_split_manifest = []
    for partition, records in (("L%d" % source_pct, source_l),
                               ("U%d" % (100-source_pct), source_u)):
        for record in records:
            item = dict(record); item["partition"] = partition
            source_split_manifest.append(item)
    target_split_manifest = []
    for partition, records in (("L%d" % target_pct, target_l),
                               ("U%d" % (100-target_pct), target_u)):
        for record in records:
            item = dict(record); item["partition"] = partition
            target_split_manifest.append(item)
    write_manifest(os.path.join(args.output_dir, "source_split_manifest.csv"),
                   source_split_manifest)
    write_manifest(os.path.join(args.output_dir, "target_split_manifest.csv"),
                   target_split_manifest)

    audit = {
        "seed": args.seed,
        "split_unit": "complete_function",
        "source_label_fraction": args.source_label_fraction,
        "target_label_fraction": args.target_label_fraction,
        "source_raw_slices": len(source_raw),
        "source_unique_nonconflicting_inputs": len(source),
        "target_raw_slices": len(target_raw),
        "target_unique_before_cross_domain_filter": len(
            [r for r in target_manifest if not r["conflict"]]),
        "target_unique_after_cross_domain_filter": len(target),
        "cross_domain_removed": len(cross_removed),
        "source_labeled_functions": len({r["function_id"] for r in source_l}),
        "source_unlabeled_functions": len({r["function_id"] for r in source_u}),
        "target_labeled_functions": len({r["function_id"] for r in target_l}),
        "target_test_functions": len({r["function_id"] for r in target_u}),
        "source_labeled_inputs": len(source_l),
        "source_unlabeled_inputs": len(source_u),
        "target_labeled_inputs": len(target_l),
        "target_test_inputs": len(target_u),
        "encoder_train_functions": len(set(model.train_function_ids)),
        "encoder_validation_functions": len(set(model.val_function_ids)),
        "shared_encoder": True,
        "encoder_backend": args.backend,
    }
    with open(os.path.join(args.output_dir, "preparation_audit.json"), "w",
              encoding="utf8") as stream:
        json.dump(audit, stream, indent=2, ensure_ascii=False)
    print(json.dumps(audit, indent=2, ensure_ascii=False))


if __name__ == "__main__":
    main()
