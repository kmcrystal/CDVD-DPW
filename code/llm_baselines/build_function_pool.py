#!/usr/bin/env python3
"""Create one-function-per-row CSV inputs for the LLM protocol.

The split manifest is produced by ``extract_transfer_features.py``.  This
utility reads complete function source from the original project directory;
it never reconstructs functions from slices.
"""

import argparse
from pathlib import Path

import pandas as pd


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--functions-root", required=True)
    parser.add_argument("--split-manifest", required=True)
    parser.add_argument("--partition", required=True,
                        choices=("L30", "U70", "L5", "U95"))
    parser.add_argument("--output", required=True)
    args = parser.parse_args()

    manifest = pd.read_csv(args.split_manifest)
    manifest = manifest[manifest["partition"] == args.partition]
    functions = manifest[["function_id", "label"]].drop_duplicates()
    conflicts = functions.groupby("function_id")["label"].nunique()
    if (conflicts > 1).any():
        raise ValueError("function-level label conflicts in split manifest")

    paths = {}
    for path in Path(args.functions_root).rglob("*.c"):
        paths.setdefault(path.name, []).append(path)
    rows = []
    for item in functions.itertuples(index=False):
        candidates = paths.get(str(item.function_id), [])
        if len(candidates) != 1:
            raise ValueError("%s resolves to %d source files" %
                             (item.function_id, len(candidates)))
        rows.append({"function_id": item.function_id,
                     "code": candidates[0].read_text(encoding="utf8",
                                                     errors="ignore"),
                     "label": int(item.label)})
    pd.DataFrame(rows).to_csv(args.output, index=False, encoding="utf8")
    print("wrote %d complete functions to %s" % (len(rows), args.output))


if __name__ == "__main__":
    main()

