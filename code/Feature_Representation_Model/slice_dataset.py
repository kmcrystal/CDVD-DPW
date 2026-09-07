"""Leakage-controlled preparation of CGD slices.

This module is shared by the single-domain compatibility entry point and the
paper-faithful source/target feature entry point.  It preserves function
identity, remaps candidate positions after comment removal, computes the three
audit hashes described in Section 3.3, consolidates exact model inputs, and
performs label-budget splits at complete-function granularity.
"""

from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path

import numpy as np
import pandas as pd

from clean_gadget import clean_gadget
from vectorize_gadget import GadgetVectorizer

SEPARATOR = "-" * 33


def _sha256(text):
    return hashlib.sha256(text.encode("utf8")).hexdigest()


def parse_cgd(path):
    """Return one metadata-rich record for every well-formed CGD slice."""
    records = []
    header = None
    body = []
    label = None

    def emit():
        if header is None or label not in (0, 1) or not body:
            return
        parts = header.split()
        if len(parts) < 4 or not parts[0].isdigit():
            raise ValueError("invalid CGD header: %s" % header)
        cand_pos = int(parts[4]) if len(parts) >= 5 else 0
        cleaned, mapped_pos = clean_gadget(body, cand_pos)
        if not cleaned:
            return
        tokens, _ = GadgetVectorizer.tokenize_gadget(cleaned, mapped_pos)
        vectorizer = GadgetVectorizer()
        model_tokens = vectorizer.model_tokens(cleaned, mapped_pos)
        records.append({
            "slice_id": int(parts[0]),
            "source_file": parts[1],
            "function_id": parts[1],
            "candidate_type": parts[2],
            "candidate_line": int(parts[3]),
            "candidate_pos_raw": cand_pos,
            "candidate_pos": mapped_pos,
            "raw_lines": list(body),
            "gadget": cleaned,
            "label": int(label),
            "raw_hash": _sha256("\n".join(body)),
            "token_hash": _sha256("\x1f".join(tokens)),
            "model_hash": _sha256("\x1f".join(model_tokens)),
        })

    with open(path, "r", encoding="utf8", errors="ignore") as stream:
        for raw in stream:
            line = raw.rstrip("\n")
            stripped = line.strip()
            if SEPARATOR in stripped:
                emit()
                header, body, label = None, [], None
            elif header is None and stripped:
                header = stripped
            elif stripped in ("0", "1"):
                label = int(stripped)
            elif header is not None:
                body.append(line)
    emit()
    return records


def consolidate_model_inputs(records):
    """Collapse exact model inputs and quarantine conflicting supervision.

    Source provenance is retained in the returned manifest.  A model input
    carrying both labels is not eligible for supervised training or testing;
    it remains visible in the manifest with ``conflict=1``.
    """
    by_hash = {}
    for record in records:
        by_hash.setdefault(record["model_hash"], []).append(record)

    parent = {r["function_id"]: r["function_id"] for r in records}

    def find(item):
        while parent[item] != item:
            parent[item] = parent[parent[item]]
            item = parent[item]
        return item

    def union(left, right):
        left, right = find(left), find(right)
        if left != right:
            parent[max(left, right)] = min(left, right)

    kept, manifest = [], []
    for model_hash, group in by_hash.items():
        labels = sorted({r["label"] for r in group})
        conflict = len(labels) > 1
        representative = dict(group[0])
        representative["duplicate_count"] = len(group)
        representative["all_slice_ids"] = [r["slice_id"] for r in group]
        representative["all_source_files"] = sorted({r["source_file"] for r in group})
        representative["all_candidate_lines"] = [r["candidate_line"] for r in group]
        representative["all_candidate_types"] = [r["candidate_type"] for r in group]
        representative["conflict"] = int(conflict)
        manifest.append(representative)
        if not conflict:
            functions = sorted({r["function_id"] for r in group})
            for function in functions[1:]:
                union(functions[0], function)
            kept.append(representative)
    for representative in kept:
        representative["split_group_id"] = find(representative["function_id"])
    for representative in manifest:
        representative["split_group_id"] = find(representative["function_id"])
    return kept, manifest


def function_group_split(records, fraction, seed=42):
    """Split complete functions, stratified by their binary function label."""
    if not 0 <= fraction <= 1:
        raise ValueError("fraction must be in [0,1]")
    function_labels = {}
    for r in records:
        group = r.get("split_group_id", r["function_id"])
        function_labels.setdefault(group, set()).add(r["label"])
    bad = {f for f, labels in function_labels.items() if len(labels) != 1}
    if bad:
        raise ValueError("function-level label conflicts: %s" % sorted(bad)[:5])

    rng = np.random.default_rng(seed)
    labeled_functions = set()
    for label in (0, 1):
        groups = sorted(f for f, labels in function_labels.items()
                        if next(iter(labels)) == label)
        if not groups:
            continue
        n_take = int(round(len(groups) * fraction))
        if fraction > 0:
            n_take = max(1, n_take)
        if fraction < 1 and len(groups) > 1:
            n_take = min(n_take, len(groups) - 1)
        if n_take:
            chosen = rng.choice(groups, size=n_take, replace=False)
            labeled_functions.update(chosen.tolist())

    labeled = [r for r in records
               if r.get("split_group_id", r["function_id"]) in labeled_functions]
    unlabeled = [r for r in records
                 if r.get("split_group_id", r["function_id"]) not in labeled_functions]
    return labeled, unlabeled


def train_vectorizer(record_sets, seed=42):
    """Train one deterministic Word2Vec vocabulary shared by all domains."""
    vectorizer = GadgetVectorizer(seed=seed)
    for records in record_sets:
        for record in records:
            vectorizer.add_gadget(record["gadget"])
    vectorizer.train_model()
    return vectorizer


def records_to_frame(records, vectorizer):
    rows = []
    for record in records:
        row = {k: v for k, v in record.items()
               if k not in ("raw_lines", "gadget")}
        row["gadget"] = record["gadget"]
        row["vector"] = vectorizer.vectorize(record["gadget"],
                                               record["candidate_pos"])
        row["val"] = record["label"]
        rows.append(row)
    return pd.DataFrame(rows)


def normalized_function_hashes(root):
    """Map each .c basename to hashes of normalized complete function text."""
    result = {}
    if not root:
        return result
    for path in Path(root).rglob("*.c"):
        text = path.read_text(encoding="utf8", errors="ignore")
        normalized = "\n".join(line.rstrip() for line in text.splitlines()).strip()
        result.setdefault(path.name, set()).add(_sha256(normalized))
    return result


def remove_cross_domain_duplicates(source, target, source_root=None,
                                   target_root=None):
    """Remove target-side exact function and model-input duplicates."""
    source_model_hashes = {r["model_hash"] for r in source}
    source_fun = normalized_function_hashes(source_root)
    target_fun = normalized_function_hashes(target_root)
    source_full_hashes = {h for values in source_fun.values() for h in values}
    duplicated_functions = {
        name for name, values in target_fun.items()
        if values & source_full_hashes
    }

    kept, removed = [], []
    for record in target:
        reason = None
        if record["function_id"] in duplicated_functions:
            reason = "exact_function_cross_domain"
        elif record["model_hash"] in source_model_hashes:
            reason = "exact_model_input_cross_domain"
        if reason:
            item = dict(record)
            item["removal_reason"] = reason
            removed.append(item)
        else:
            kept.append(record)
    return kept, removed


def write_manifest(path, records):
    """Write scalar audit fields; provenance lists are JSON encoded."""
    rows = []
    for record in records:
        row = {}
        for key, value in record.items():
            if key in ("gadget", "raw_lines", "vector"):
                continue
            if isinstance(value, set):
                value = sorted(value)
            row[key] = json.dumps(value, ensure_ascii=False) \
                if isinstance(value, (list, dict)) else value
        rows.append(row)
    pd.DataFrame(rows).to_csv(path, index=False, encoding="utf8")
