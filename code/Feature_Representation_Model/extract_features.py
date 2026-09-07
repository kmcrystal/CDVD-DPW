"""
extract_features.py -- Feature Representation Model driver (paper Section 3.3)

Pipeline for one domain (one cgd slice file produced by gen_slice):

  1. Parse the cgd file into gadgets (program slices + labels
     + candidate point position).
  2. Symbolize identifiers with clean_gadget (VAR1/FUN1 mapping).
  3. Train Word2Vec (50-d) and build candidate-point-centered
     500-token embedding sequences.
  4. Supervised pre-training of the BiLSTM (h=64, global max pooling)
     with cross-entropy on the labeled fraction only.
  5. Extract 128-d feature vectors z for ALL samples and export:

        <base>_<frac>%_labels_features_ext.mat    labeled split  (L_s)
        <base>_<100-frac>%_labels_features_ext.mat unlabeled split (U_s)
        <base>_all_labels_features_ext.csv         everything (inspection)

     Each .mat contains a single variable `data` = [features, label]
     ([n x 129]) -- the format consumed by the MATLAB DSLP module.

Usage:
    # source domain, 30% labels (paper default)
    python extract_features.py libpng_cgd.txt 0.30 true

    # target domain, 5% labels (paper default)
    python extract_features.py vlc_cgd.txt 0.05 false

Arguments: [cgd file] [labeled fraction] [undersample true|false]
"""

import json
import os
import sys

import numpy as np
import pandas
import scipy.io

from blstm import BLSTM
from slice_dataset import (consolidate_model_inputs, function_group_split,
                           parse_cgd, records_to_frame, train_vectorizer,
                           write_manifest)

SEED = 42


def get_vectors_df(filename):
    """Parse, audit, deduplicate and vectorize one CGD domain."""
    parsed = parse_cgd(filename)
    unique, manifest = consolidate_model_inputs(parsed)
    vectorizer = train_vectorizer([unique], seed=SEED)
    df = records_to_frame(unique, vectorizer)
    print("slices: %d, unique model inputs: %d, conflicts: %d" %
          (len(parsed), len(unique), sum(r["conflict"] for r in manifest)))
    return df, manifest


def split_labeled_unlabeled(df, frac, seed=SEED):
    """Stratified complete-function split into L and U."""
    records = df.to_dict("records")
    labeled, unlabeled = function_group_split(records, frac, seed)
    return pandas.DataFrame(labeled), pandas.DataFrame(unlabeled)


def save_mat(path, features, labels):
    """Save [features, label] as variable `data` for the MATLAB modules."""
    data = np.hstack((features, labels.reshape(-1, 1).astype(float)))
    scipy.io.savemat(path, {"data": data})
    print("Saved", path, "shape", data.shape)


def main():
    if len(sys.argv) != 4:
        print(__doc__)
        sys.exit(1)
    filename = sys.argv[1]
    frac = float(sys.argv[2])
    undersample = sys.argv[3].lower() == "true"

    base = os.path.splitext(os.path.basename(filename))[0]
    vector_filename = base + "_gadget_vectors_v2.pkl"
    manifest_filename = base + "_slice_manifest.csv"
    if os.path.exists(vector_filename):
        df = pandas.read_pickle(vector_filename)
        manifest = None
    else:
        df, manifest = get_vectors_df(filename)
        df.to_pickle(vector_filename)
        write_manifest(manifest_filename, manifest)

    labeled_df, unlabeled_df = split_labeled_unlabeled(df, frac)
    print("labeled: %d  unlabeled: %d" % (len(labeled_df), len(unlabeled_df)))

    # Supervised pre-training on the labeled fraction only (paper 3.3.3)
    blstm = BLSTM(labeled_df, name="%s_%g" % (base, frac),
                  enable_undersampling=undersample, seed=SEED)
    blstm.dir = base
    os.makedirs(blstm.dir, exist_ok=True)
    split_audit = {
        "seed": SEED,
        "split_unit": "complete_function",
        "labeled_fraction": frac,
        "labeled_functions": sorted(labeled_df["function_id"].unique().tolist()),
        "unlabeled_functions": sorted(unlabeled_df["function_id"].unique().tolist()),
        "labeled_model_inputs": len(labeled_df),
        "unlabeled_model_inputs": len(unlabeled_df),
    }
    with open(os.path.join(blstm.dir, "split_audit.json"), "w",
              encoding="utf8") as stream:
        json.dump(split_audit, stream, ensure_ascii=False, indent=2)
    blstm.train()
    blstm.test()

    # Extract 128-d features for both splits with the SAME encoder so the
    # downstream modules operate in a single unified feature space.
    pct = int(round(frac * 100))
    for split_df, tag in ((labeled_df, "%d%%" % pct),
                          (unlabeled_df, "%d%%" % (100 - pct)),
                          (df, "all")):
        features, labels = blstm.extract_features(split_df)
        if tag == "all":
            blstm.obtain(split_df, "%s_all" % base)
        else:
            save_mat(os.path.join(blstm.dir,
                                  "%s_labels_features_ext.mat" % tag),
                     features, labels)


if __name__ == "__main__":
    main()
