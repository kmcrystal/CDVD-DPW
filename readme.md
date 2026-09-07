# CDVD-DPW Replication Artifact

This repository provides the replication artifact for **CDVD-DPW: Cross-Domain Vulnerability Detection via Semi-Supervised Learning**.

The artifact is organized into three parts: the implementation, the evaluation datasets, and supplementary evidence referenced in the paper.

## Artifact Contents

| Resource                              | Description                                                  |
| ------------------------------------- | ------------------------------------------------------------ |
| **[Code](./code/)**                   | Implementation of CDVD-DPW, installation instructions, implementation-to-paper alignment, and executable LLM baselines |
| **[Dataset](./dataset/)**             | Dataset documentation, provenance information, and access to the complete archived evaluation datasets |
| **[Supplementary](./supplementary/)** | Supplementary evidence referenced by the evaluation, including the extended RQ2 case audit |

## Code

The [`code/`](./code/) directory contains the complete implementation of the CDVD-DPW pipeline:

1. vulnerability-oriented program slicing;
2. feature representation learning;
3. Dual-Stage Label Propagation (DLP);
4. Dual-Factor Weighted Cross-Domain Representation Learning (DFW);
5. classification and evaluation; and
6. LLM baseline evaluation.

Detailed installation, execution, and implementation-to-paper mappings are provided inside the code directory.

[Open the implementation](./code/)

## Dataset

The [`dataset/`](./dataset/) directory documents the datasets used for the cross-CWE and cross-project experiments, including their construction and provenance.

The complete evaluation datasets are permanently archived on Zenodo:

**DOI:** [10.5281/zenodo.22305410](https://doi.org/10.5281/zenodo.22305410)

The Zenodo archive contains the released dataset package together with its provenance, evidence, and manifest information.

[Open the dataset documentation](./dataset/)

## Supplementary Evidence

The [`supplementary/`](./supplementary/) directory contains additional evidence explicitly referenced in the paper.

In particular, the RQ2 case audit supplements the representative cross-project cases reported in the manuscript with the complete set of:

- true-positive (TP);
- true-negative (TN);
- false-positive (FP); and
- false-negative (FN) cases.

The case materials provide the corresponding target contexts and weighted source evidence used in the qualitative analysis.

[Open the supplementary materials](./supplementary/)

## Artifact Organization

```text
.
├── README.md
├── code/
│   ├── implementation and execution documentation
│   ├── vulnerability-oriented representation pipeline
│   ├── Dual-Stage Label Propagation
│   ├── Dual-Factor Weighted Cross-Domain Representation Learning
│   ├── llm_baselines/
│   └── tests/
│
├── dataset/
│   └── dataset documentation and Zenodo archive information
│
└── supplementary/
    └── RQ2 case audit
```