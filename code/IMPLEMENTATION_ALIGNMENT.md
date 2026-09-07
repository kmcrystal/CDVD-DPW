# Current-manuscript implementation alignment

This note records the implementation contracts enforced by the code. It is an
audit aid, not an experimental-results file.

## Data and representation

- Reported slices are generated with Joern 2.0.195/c2cpg. The lightweight
  slicer is retained only for smoke tests.
- Comment removal preserves executable text adjoining comments and remaps the
  candidate-line position after empty lines are removed.
- Every slice receives a raw-source hash, normalized-token hash and exact
  candidate-centered 500-token model-input hash.
- Identical model inputs are consolidated before feature encoding. Inputs
  carrying conflicting binary labels are quarantined and retained in the
  audit manifest, not used as supervision or evaluation instances.
- Label budgets are assigned to complete C functions. No function contributes
  slices to both sides of a labeled/unlabeled boundary.
- For a directed transfer, exact target functions duplicated in the source are
  removed first, followed by remaining target model inputs duplicated in the
  source. Every removal is recorded.
- Source and target are encoded with one deterministic Word2Vec model and one
  BiLSTM. The directed-transfer entry defaults to the manuscript's PyTorch
  backend; the historical TensorFlow backend remains selectable for existing
  environments. Both produce the same 64-per-direction/global-max-pooling
  architecture and a shared 128-dimensional representation.

## DLP

- One adaptively selected neighborhood size `k` is shared by Stage I and Stage
  II, following the current manuscript.
- Effective rho/tau thresholds are retained for the Stage-II acceptance rule.
- Class-mass-normalized confidence is used consistently for both the final PAL
  label and its confidence.
- A PAL representation joins `D_p` only when calibrated confidence, reliable
  neighbor agreement and final-iteration stability all pass. Unresolved PAL
  representations remain unlabeled.
- The output MAT contains counts, coverage, offline pseudo-label accuracies,
  effective thresholds, selected `k`, resolved indices and unresolved indices.
  Hidden U70 labels are accessed only for these offline accuracy fields.
- DLP normalizes copies for within-source neighborhood calculations but exports
  the original shared-encoder coordinates to DFW.

## DFW and classifier

- The unreported 100-dimensional PCA step was removed. DFW operates on the
  shared 128-dimensional representation described by the manuscript.
- Density-ratio bandwidth estimation uses a bounded deterministic sample.
- Manifold consistency uses KNN queries instead of quadratic full distance
  matrices, allowing the reported CWE-scale inputs to run without excessive
  memory.
- Unlabeled target representations may characterize the target distribution;
  their labels are not read until final metric calculation.
- The result structure retains learned weights, dual-factor priors, metric,
  predictions, ground truth and effective parameters for audit.

## LLM comparison

- Model identifiers are frozen as `gpt-4o-2024-08-06`, `gemini-2.5-pro`, and
  `grok-4-0709`.
- The test panel contains 500 frozen complete target functions.
- Ten configurations are generated as five stratified folds times two
  independently selected demonstration sets. Each prompt has three source and
  two target demonstrations, ordered V/S/V/S/V.
- All providers receive the same prompt and the same function text. Functions
  over 6,000 `o200k_base` tokens are truncated from the tail.
- Decoding uses temperature 0, top-p 1 and at most eight output tokens. Only an
  exact case-insensitive `VULNERABLE` or `SAFE` response parses. A fixed retry
  prompt is issued once; remaining failures receive the majority label of the
  available labeled pools.
- Request-level audit JSONL retains timestamps, latencies, raw outputs,
  response IDs, usage metadata, retry outcomes and prompt hashes, but never API
  keys. Metrics are written per configuration and as mean/SD over all ten.

## Validation status

- Python sources pass `py_compile`.
- Regression tests cover comment/candidate mapping, model-input conflict
  quarantine, complete-function partitioning, and exact five-example prompt
  topology.
- MATLAB execution still requires MATLAB plus Statistics and Machine Learning
  Toolbox on the Linux experiment server; the Windows editing host does not
  provide MATLAB/Octave, so runtime numerical validation must be performed
  there before reporting new measurements.
