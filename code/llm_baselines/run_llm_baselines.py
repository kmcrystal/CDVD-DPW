#!/usr/bin/env python3
"""Reproduce the paper's fixed 500-function, 5-shot LLM comparison.

Input CSV files require ``function_id``, ``code`` and binary ``label``
columns.  Source and target demonstration pools must be the already frozen
30% and 5% labeled function pools; the test CSV must be disjoint from both.
API keys are read only from OPENAI_API_KEY, GEMINI_API_KEY and XAI_API_KEY.
They are never written to an audit artifact.
"""

from __future__ import annotations

import argparse
import datetime as dt
import hashlib
import json
import os
import random
import time
import urllib.error
import urllib.parse
import urllib.request
from pathlib import Path

import numpy as np
import pandas as pd
import tiktoken
from sklearn.metrics import (f1_score, matthews_corrcoef,
                             precision_score, recall_score)
from sklearn.model_selection import StratifiedKFold

from prompt_template import (RETRY_INSTRUCTION, SYSTEM_INSTRUCTION,
                             build_user_prompt)

MODELS = {
    "gpt-4o-2024-08-06": "openai",
    "gemini-2.5-pro": "gemini",
    "grok-4-0709": "xai",
}
TOKENIZER_NAME = "o200k_base"
MAX_FUNCTION_TOKENS = 6000


def read_pool(path):
    frame = pd.read_csv(path)
    required = {"function_id", "code", "label"}
    missing = required - set(frame.columns)
    if missing:
        raise ValueError("%s missing columns %s" % (path, sorted(missing)))
    frame = frame.copy()
    frame["function_id"] = frame["function_id"].astype(str)
    frame["label"] = frame["label"].astype(int)
    if not set(frame["label"]).issubset({0, 1}):
        raise ValueError("labels must be binary")
    if frame["function_id"].duplicated().any():
        raise ValueError("function_id must be unique in %s" % path)
    return frame


def truncate_function(code, encoder, limit=MAX_FUNCTION_TOKENS):
    tokens = encoder.encode(str(code))
    truncated = len(tokens) > limit
    if truncated:
        tokens = tokens[:limit]  # documented tail truncation
    return encoder.decode(tokens), len(tokens), truncated


def fixed_test_panel(frame, n, seed):
    if len(frame) < n:
        raise ValueError("test pool has %d functions; %d required" % (len(frame), n))
    if len(frame) == n:
        return frame.sort_values("function_id").reset_index(drop=True)
    parts = []
    for label, group in frame.groupby("label"):
        take = int(round(n * len(group) / len(frame)))
        parts.append(group.sample(n=take, random_state=seed + int(label)))
    panel = pd.concat(parts)
    if len(panel) > n:
        panel = panel.sample(n=n, random_state=seed)
    elif len(panel) < n:
        remainder = frame.drop(panel.index)
        panel = pd.concat([panel, remainder.sample(n=n-len(panel), random_state=seed)])
    return panel.sort_values("function_id").reset_index(drop=True)


def fold_candidates(frame, seed):
    splitter = StratifiedKFold(n_splits=5, shuffle=True, random_state=seed)
    folds = []
    dummy = np.zeros(len(frame))
    for _, idx in splitter.split(dummy, frame["label"]):
        folds.append(frame.iloc[idx].reset_index(drop=True))
    return folds


def sample_label(frame, label, n, rng):
    rows = frame[frame["label"] == label]
    if len(rows) < n:
        raise ValueError("fold lacks %d examples of label %d" % (n, label))
    indices = rng.sample(list(rows.index), n)
    return [rows.loc[i].to_dict() for i in indices]


def build_configurations(source, target, seed):
    source_folds = fold_candidates(source, seed)
    target_folds = fold_candidates(target, seed + 1)
    configs = []
    for fold in range(5):
        for repetition in range(2):
            rng = random.Random(seed + fold * 100 + repetition)
            sv = sample_label(source_folds[fold], 1, 2, rng)
            ss = sample_label(source_folds[fold], 0, 1, rng)
            tv = sample_label(target_folds[fold], 1, 1, rng)
            ts = sample_label(target_folds[fold], 0, 1, rng)
            # Alternating V/S order, with exactly three source and two target
            # functions and the protocol's 3-vulnerable/2-safe composition.
            demos = [sv[0], ts[0], sv[1], ss[0], tv[0]]
            domains = ["source", "target", "source", "source", "target"]
            for item, domain in zip(demos, domains):
                item["domain"] = domain
            configs.append({
                "config_id": "fold%d_set%d" % (fold + 1, repetition + 1),
                "fold": fold + 1,
                "repetition": repetition + 1,
                "demonstrations": demos,
            })
    return configs


def request_json(url, headers, payload, attempts=5):
    body = json.dumps(payload).encode("utf8")
    for attempt in range(attempts):
        request = urllib.request.Request(url, data=body, headers=headers,
                                         method="POST")
        try:
            with urllib.request.urlopen(request, timeout=120) as response:
                return json.loads(response.read().decode("utf8"))
        except urllib.error.HTTPError as error:
            if error.code not in (408, 409, 429, 500, 502, 503, 504) or attempt == attempts-1:
                detail = error.read().decode("utf8", errors="replace")
                raise RuntimeError("HTTP %s: %s" % (error.code, detail[-1000:]))
            time.sleep(min(2 ** attempt, 16))


def call_openai_compatible(model, provider, messages):
    if provider == "openai":
        key = os.environ["OPENAI_API_KEY"]
        url = "https://api.openai.com/v1/chat/completions"
    else:
        key = os.environ["XAI_API_KEY"]
        url = "https://api.x.ai/v1/chat/completions"
    payload = {"model": model, "messages": messages, "temperature": 0,
               "top_p": 1, "max_tokens": 8}
    response = request_json(url, {"Authorization": "Bearer " + key,
                                  "Content-Type": "application/json"}, payload)
    return response["choices"][0]["message"]["content"], response


def call_gemini(model, messages):
    key = os.environ["GEMINI_API_KEY"]
    url = ("https://generativelanguage.googleapis.com/v1beta/models/%s:generateContent?key=%s" %
           (urllib.parse.quote(model), urllib.parse.quote(key)))
    contents = []
    for message in messages:
        if message["role"] == "system":
            continue
        role = "model" if message["role"] == "assistant" else "user"
        contents.append({"role": role, "parts": [{"text": message["content"]}]})
    payload = {
        "systemInstruction": {"parts": [{"text": SYSTEM_INSTRUCTION}]},
        "contents": contents,
        "generationConfig": {"temperature": 0, "topP": 1,
                             "maxOutputTokens": 8},
    }
    response = request_json(url, {"Content-Type": "application/json"}, payload)
    text = response["candidates"][0]["content"]["parts"][0]["text"]
    return text, response


def parse_label(raw):
    normalized = str(raw).strip().upper()
    if normalized == "VULNERABLE":
        return 1
    if normalized == "SAFE":
        return 0
    return None


def infer(model, prompt, majority_label):
    provider = MODELS[model]
    messages = [{"role": "system", "content": SYSTEM_INSTRUCTION},
                {"role": "user", "content": prompt}]
    attempts = []
    for attempt in (1, 2):
        started = time.time()
        if provider == "gemini":
            raw, response = call_gemini(model, messages)
        else:
            raw, response = call_openai_compatible(model, provider, messages)
        parsed = parse_label(raw)
        attempts.append({"attempt": attempt, "raw_output": raw,
                         "parsed": parsed,
                         "timestamp_utc": dt.datetime.now(dt.timezone.utc).isoformat(),
                         "latency_seconds": time.time() - started,
                         "response_id": response.get("id", response.get("responseId")),
                         "usage": response.get("usage", response.get("usageMetadata", {}))})
        if parsed is not None:
            return parsed, False, attempts
        messages.extend([{"role": "assistant", "content": raw},
                         {"role": "user", "content": RETRY_INSTRUCTION}])
    return majority_label, True, attempts


def metrics(group):
    truth = group["label"].astype(int)
    pred = group["prediction"].astype(int)
    return {
        "precision": precision_score(truth, pred, zero_division=0),
        "recall": recall_score(truth, pred, zero_division=0),
        "f1": f1_score(truth, pred, zero_division=0),
        "mcc": matthews_corrcoef(truth, pred),
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-pool", required=True)
    parser.add_argument("--target-pool", required=True)
    parser.add_argument("--test-pool", required=True)
    parser.add_argument("--output-dir", required=True)
    parser.add_argument("--test-size", type=int, default=500)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--models", nargs="+", choices=sorted(MODELS),
                        default=list(MODELS))
    parser.add_argument("--execute", action="store_true",
                        help="perform paid API calls; otherwise only freeze prompts")
    parser.add_argument("--resume", action="store_true",
                        help="resume from predictions.partial.jsonl")
    args = parser.parse_args()

    output = Path(args.output_dir)
    output.mkdir(parents=True, exist_ok=True)
    source = read_pool(args.source_pool)
    target = read_pool(args.target_pool)
    test = fixed_test_panel(read_pool(args.test_pool), args.test_size, args.seed)
    if set(test.function_id) & (set(source.function_id) | set(target.function_id)):
        raise ValueError("test functions overlap a labeled demonstration pool")

    encoder = tiktoken.get_encoding(TOKENIZER_NAME)
    test = test.copy()
    trunc = [truncate_function(code, encoder) for code in test["code"]]
    test["code"] = [x[0] for x in trunc]
    test["input_tokens"] = [x[1] for x in trunc]
    test["truncated"] = [x[2] for x in trunc]
    test.drop(columns=["code"]).to_csv(output / "fixed_test_panel.csv", index=False)

    configs = build_configurations(source, target, args.seed)
    for config in configs:
        for demo in config["demonstrations"]:
            code, count, was_truncated = truncate_function(demo["code"], encoder)
            demo["code"] = code
            demo["input_tokens"] = count
            demo["truncated"] = was_truncated
    public_configs = []
    for config in configs:
        public_configs.append({
            "config_id": config["config_id"], "fold": config["fold"],
            "repetition": config["repetition"],
            "demonstrations": [{k: d[k] for k in ("function_id", "label", "domain",
                                                    "input_tokens", "truncated")}
                               for d in config["demonstrations"]],
        })
    (output / "prompt_configurations.json").write_text(
        json.dumps(public_configs, indent=2, ensure_ascii=False), encoding="utf8")
    protocol = {
        "models": args.models,
        "system_instruction": SYSTEM_INSTRUCTION,
        "retry_instruction": RETRY_INSTRUCTION,
        "tokenizer": TOKENIZER_NAME,
        "max_function_tokens": MAX_FUNCTION_TOKENS,
        "truncation": "tail",
        "temperature": 0,
        "top_p": 1,
        "max_output_tokens": 8,
        "test_functions": len(test),
        "prompt_configurations": len(configs),
        "seed": args.seed,
    }
    (output / "protocol.json").write_text(
        json.dumps(protocol, indent=2), encoding="utf8")
    if not args.execute:
        print("Protocol frozen in %s; add --execute to perform API calls." % output)
        return

    majority_label = int(pd.concat([source, target])["label"].mode().iloc[0])
    predictions = []
    audit_path = output / "request_audit.jsonl"
    partial_path = output / "predictions.partial.jsonl"
    completed = {}
    if args.resume and partial_path.exists():
        for line in partial_path.read_text(encoding="utf8").splitlines():
            item = json.loads(line)
            key = (item["model"], item["config_id"], item["function_id"])
            completed[key] = item
        predictions.extend(completed.values())
    audit_mode = "a" if args.resume else "w"
    partial_mode = "a" if args.resume else "w"
    with audit_path.open(audit_mode, encoding="utf8") as audit, \
            partial_path.open(partial_mode, encoding="utf8") as partial:
        for model in args.models:
            for config in configs:
                for _, row in test.iterrows():
                    key = (model, config["config_id"], row["function_id"])
                    if key in completed:
                        continue
                    prompt = build_user_prompt(config["demonstrations"], row["code"])
                    prompt_hash = hashlib.sha256(prompt.encode("utf8")).hexdigest()
                    pred, fallback, attempts = infer(model, prompt, majority_label)
                    item = {"model": model, "config_id": config["config_id"],
                            "function_id": row["function_id"],
                            "label": int(row["label"]), "prediction": pred,
                            "majority_fallback": fallback,
                            "prompt_sha256": prompt_hash, "attempts": attempts}
                    audit.write(json.dumps(item, ensure_ascii=False) + "\n")
                    audit.flush()
                    prediction = {k: item[k] for k in
                                  ("model", "config_id", "function_id",
                                   "label", "prediction", "majority_fallback")}
                    predictions.append(prediction)
                    partial.write(json.dumps(prediction, ensure_ascii=False) + "\n")
                    partial.flush()

    pred_frame = pd.DataFrame(predictions)
    pred_frame.to_csv(output / "predictions.csv", index=False)
    per_config = []
    for (model, config_id), group in pred_frame.groupby(["model", "config_id"]):
        row = {"model": model, "config_id": config_id}
        row.update(metrics(group))
        row["unparseable_after_retry"] = int(group["majority_fallback"].sum())
        per_config.append(row)
    per_config = pd.DataFrame(per_config)
    per_config.to_csv(output / "metrics_per_configuration.csv", index=False)
    summary = per_config.groupby("model")[["precision", "recall", "f1", "mcc"]].agg(
        ["mean", "std"])
    summary.to_csv(output / "metrics_summary.csv")
    print(summary)


if __name__ == "__main__":
    main()
