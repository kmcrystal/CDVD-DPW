# LLM baseline reproduction

This directory implements the exact protocol described in paper Section 4.3:
the fixed 500-function CWE-399 panel, five demonstrations (three from the
30%-labeled source-function pool and two from the 5%-labeled target-function
pool), five folds with two independently sampled demonstration sets per fold,
and deterministic decoding for `gpt-4o-2024-08-06`, `gemini-2.5-pro`, and
`grok-4-0709`.

Input CSV files contain one complete function per row and the columns
`function_id,code,label`. The target test pool must be disjoint from both
labeled pools. Tail truncation uses `tiktoken`'s `o200k_base` encoding and a
6,000-token function limit for all providers, ensuring an identical textual
input rather than provider-specific truncation.

Build these CSVs from the grouped split manifests and original function trees:

```bash
python build_function_pool.py --functions-root ../CWE119 \
  --split-manifest ../runs/119_to_399/source_split_manifest.csv \
  --partition L30 --output cwe119_L30_functions.csv
python build_function_pool.py --functions-root ../CWE399 \
  --split-manifest ../runs/119_to_399/target_split_manifest.csv \
  --partition L5 --output cwe399_L5_functions.csv
python build_function_pool.py --functions-root ../CWE399 \
  --split-manifest ../runs/119_to_399/target_split_manifest.csv \
  --partition U95 --output cwe399_U95_functions.csv
```

First freeze and inspect the protocol without making paid calls:

```bash
python run_llm_baselines.py \
  --source-pool cwe119_L30_functions.csv \
  --target-pool cwe399_L5_functions.csv \
  --test-pool cwe399_U95_functions.csv \
  --output-dir results/cwe119_to_cwe399
```

Then provide keys through environment variables and add `--execute`:

```bash
export OPENAI_API_KEY='...'
export GEMINI_API_KEY='...'
export XAI_API_KEY='...'
python run_llm_baselines.py ... --execute
```

Keys are never read from a repository file or included in request audits.
The audit records raw outputs, retries, provider usage metadata, latencies,
prompt hashes, parsed labels, and any majority-label fallback. Metrics are
reported per prompt configuration and as mean/standard deviation across the
ten configurations. If a paid run is interrupted, rerun the same command with
`--execute --resume`; completed model/configuration/function triples are read
from `predictions.partial.jsonl` and are not billed again.
