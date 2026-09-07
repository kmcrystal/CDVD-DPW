# CDVD-DPW — reproducibility implementation (`forpaper`)

This folder implements **CDVD-DPW: Cross-Domain
Vulnerability Detection with Limited Source Labels via Dual Propagation and
Dual-Factor Weighting**. The directed-transfer entry points enforce the
function-level partitioning, exact deduplication, shared representation and
audit protocol described in the current manuscript.

```
forpaper/
  LibPNG/, LibTIFF/                     sample data: function-level .c files
      Vulnerable_functions/  (label 1)
      Non_vulnerable_functions/ (label 0)

  gen_slice/                                     Module 1 (paper §3.3.1-3.3.2)
      gen_slice.py            candidate points + PDG + program slicing

  Feature_Representation_Model/                  Module 2 (paper §3.3.3)
      clean_gadget.py         identifier symbolization (VAR1/FUN1)
      vectorize_gadget.py     Word2Vec 50-d, 500-token centered sequences
      blstm_torch.py          paper backend: PyTorch BiLSTM -> z ∈ R^128
      blstm.py                TensorFlow compatibility backend
      slice_dataset.py        hashes, conflicts, grouped splits, manifests
      extract_transfer_features.py shared source/target encoder (main entry)
      extract_features.py     legacy single-domain compatibility entry

  Dual-Stage Label Propagation Mechanism/        Module 3 (paper §3.4)   [MATLAB]
      dslp_main.m             pipeline entry (Stage I + Stage II)
      adaptive_k_selection.m  dynamic k (Eq. 19)
      stage1_knn_influence.m  KNN influence pseudo-labeling (Eq. 2, Eq. 20)
      reconstruction_weights.m propagation weight matrices (Eq. 3)
      stage2_multisource_propagation.m  hierarchical propagation (Eq. 4-11)
      load_features.m

  Dual-Factor Weighted Cross-Domain Representation Learning/
                                                 Module 4 (paper §3.5)   [MATLAB]
      cdr_main.m              pipeline entry
      compute_density_ratio.m Factor 1: r(x)=P_T(x)/P_S(x)  (Eq. 13-14)
      manifold_consistency.m  Factor 2: MC(x)               (Eq. 1)
      adaptive_lambda_mu.m    lambda/mu from MMD            (Eq. 21)
      compute_MMD.m
      metric_transfer.m       metric + weight optimization  (Eq. 16-18)
      balanced_rf_classify.m  Balanced Random Forest        (§3.5.5)
      evaluate_metrics.m      Precision / Recall / F1 / MCC (§4.3)
      PCA_reduce.m

  llm_baselines/                              Paper §4.2-4.3
      prompt_template.py       exact system/user/retry prompt text
      build_function_pool.py   complete-function CSV construction
      run_llm_baselines.py     500-function, 10-configuration API evaluation

  tests/
      test_data_protocol.py    grouping/dedup/comment/prompt regression tests
```

## End-to-end workflow

The pipeline follows Figure 1 of the paper. Steps 1-2 are Python, steps 3-4
are MATLAB. The paper's default label budget is **30% source labels + 5%
target labels**; the remaining 95% of the target domain is the test set.

### Step 1 — Program slicing (both domains)

```bash
cd gen_slice
python gen_slice_joern.py ../LibPNG  libpng_cgd.txt
python gen_slice_joern.py ../LibTIFF libtiff_cgd.txt
```

For every function, candidate suspicious points of four classes are located
(API calls, array usage, pointer operations, arithmetic expressions), a
statement-level Program Dependence Graph is built (data + control
dependencies), and the forward ∪ backward slice around each candidate point
becomes one sample. Labels are inherited from the folder
(`Vulnerable_functions` = 1). Note: the full project builds
PDGs are constructed with Joern 2.0.195/c2cpg. `gen_slice.py` remains a
dependency-free smoke-test slicer and must not be substituted for Joern in
reported experiments. Every Joern run writes `<output>.failures.jsonl`; failed
files are never silently omitted.

### Step 2 — Feature representation (both domains)

```bash
cd ../Feature_Representation_Model
python extract_transfer_features.py \
  ../gen_slice/libpng_cgd.txt ../gen_slice/libtiff_cgd.txt \
  ../runs/libpng_to_libtiff \
  --source-functions ../LibPNG --target-functions ../LibTIFF \
  --source-label-fraction 0.30 --target-label-fraction 0.05 --seed 42 \
  --backend torch
```

Before encoding, the driver computes `raw_hash`, `token_hash` and
`model_hash`; exact 500-token inputs are consolidated and conflicting-label
inputs are quarantined. Target functions exactly duplicated in the source and
remaining cross-domain model-input duplicates are removed from the target.
The 30/70 and 5/95 allocations are stratified by complete function, so every
slice of one function remains on the same side. A single deterministic
Word2Vec model and BiLSTM encoder is then shared by source and target, making
all 128-dimensional vectors directly comparable.

Outputs under `runs/<direction>/source` and `target` use variable
`data = [features, label]` (n x 129). The run root also contains split,
deduplication, conflict and cross-domain-removal manifests.

| file | content |
|---|---|
| `30%_labels_features_ext.mat` | source labeled split `L_s` |
| `70%_labels_features_ext.mat` | source unlabeled split `U_s` |
| `5%_labels_features_ext.mat`  | target labeled split `L_t` |
| `95%_labels_features_ext.mat` | target test set |

### Step 3 — Dual-stage label propagation (source domain, MATLAB)

```matlab
addpath('Dual-Stage Label Propagation Mechanism');
dslp_main('runs/libpng_to_libtiff/source/30%_labels_features_ext.mat', ...
          'runs/libpng_to_libtiff/source/70%_labels_features_ext.mat', ...
          'extended_training_set.mat');
```

*Stage I* (Eq. 2): each unlabeled sample receives Gaussian-weighted class
influence from its k nearest labeled neighbors (σ = median pairwise
distance in `L_s`; k chosen by Eq. 19 consistency search starting from
√|L_s|). Thresholds ρ=0.9 / τ=0.7 (optionally entropy-adapted, Eq. 20)
split `U_s` into high-confidence pseudo-labeled (HCP → `D_h`),
partially-labeled (PAL), and discarded (UL) samples.

*Stage II*: a heterogeneous graph over V_L / V_H / V_P with four
reconstruction-weight matrices (W_pl, W_hl, W_ph, W_pp; no reverse V_P→V_H
path) is iterated with hierarchical priorities α=0.7 > β=0.45 > γ=0.25 >
δ=0.1 until ‖ΔF‖ < 1e-4 or 50 iterations. Class-mass calibrated PAL nodes
join `D_p` only if calibrated confidence, reliable-neighbor agreement and
final-iteration label stability pass; unresolved PAL nodes remain unlabeled.
Stage I and Stage II use the same adaptively selected `k`, as specified in the
current manuscript.

Output: `extended_training_set.mat` = `L_s ∪ D_h ∪ D_p` with per-sample
provenance and confidence.

### Step 4 — Dual-factor weighted transfer + detection (MATLAB)

```matlab
addpath('Dual-Factor Weighted Cross-Domain Representation Learning');
[metrics, result] = cdr_main('extended_training_set.mat', ...
                             '5%_labels_features_ext.mat', ...
                             '95%_labels_features_ext.mat');
```

1. **Factor 1** — density ratio r(x) via Gaussian-basis least-squares fitting
   (b = min(100, |D_T|/10) centers from the target, σ_rbf = median
   source-target distance, λ_r = 0.01), normalized to [0,1].
2. **Factor 2** — manifold consistency MC(x) with k_MC = 15, ω = 0.6.
3. **Dual-factor prior** w0(x) = r(x)·MC(x) — a strict conjunction: only
   samples that are both globally relevant and locally stable get weight.
4. λ/μ set adaptively from MMD and Var(w0).
5. Alternating optimization of the Mahalanobis metric A and weights w
   minimizing tr(AᵀA) + λ‖w−w0‖² + μ(ℓ_in − ℓ_out), s.t. Σw_S = n_S, w ≥ 0.
6. All features transformed (z̃ = Az); a Balanced Random Forest (100 trees,
   per-tree class-balanced bootstrap weighted by w) classifies the target
   test set. Reported metrics: Precision, Recall, F1, MCC (primary).

Large-neighborhood calculations use bounded bandwidth subsampling and KNN
queries rather than materializing quadratic source-source/source-target
distance matrices.

### LLM comparison

`llm_baselines/run_llm_baselines.py` freezes the exact paper protocol before
any paid request. It evaluates the fixed 500-function target panel using five
folds and two independently sampled demonstration sets per fold. Each prompt
contains three labeled source functions and two labeled target functions in
alternating 3-vulnerable/2-safe order. All providers receive the same text,
tail-truncated to 6,000 tokens with `o200k_base`; decoding uses temperature 0,
top-p 1 and at most 8 output tokens. Exact `VULNERABLE`/`SAFE` parsing, the
single retry, fallback, latency, usage and raw output are recorded per request.
See `llm_baselines/README.md` for commands and environment variables.

## Paper defaults recap

| Parameter | Value | Where |
|---|---|---|
| feature dim l | 128 (BiLSTM 2h, h=64) | §3.3.3 |
| token sequence | 500, candidate-centered | §3.3.2 |
| embeddings | Word2Vec, 50-d | §3.3.3 |
| ρ / τ | 0.9 / 0.7 | §3.4.2 |
| α / β / γ / δ | 0.7 / 0.45 / 0.25 / 0.1 | §3.4.3, Table 9 |
| Stage II stop | ε=1e-4, T_max=50 | Eq. 9 |
| k_MC / ω | 15 / 0.6 | §2.3 |
| λ_r | 0.01 | Eq. 14 |
| λ/μ ratio | clip(MMD/Var(w0), [0.5,10]) | Eq. 21 |
| metric T_max | 100, tol 1e-4 | §3.5.3 |
| labels | 30% source, 5% target | §4.4.2 |

## Requirements

- **Paper backend**: Python 3.8-3.10 and `requirements_torch.txt`
- **Legacy TensorFlow backend**: Python 3.10/3.11 and `requirements.txt`
- **MATLAB R2018a+** with the Statistics and Machine Learning Toolbox
  (`KDTreeSearcher`, `knnsearch`, `fitctree`)
