# CDVD-DPW Cross-Domain Vulnerability Dataset

Version 1.0.0

This release supports the cross-project and cross-vulnerability-type experiments in **“CDVD-DPW: Cross-Domain Vulnerability Detection with Limited Source Labels via Dual Propagation and Dual-Factor Weighting.”** It contains source artifacts and provenance metadata; it does not contain trained models, learned embeddings, or experimental predictions.

## Start here

| Location | Purpose |
|---|---|
| `data/cross_project/` | Six project domains distributed as one archive per project. |
| `data/cross_cwe/public_vuldeepecker/` | Public legacy CWE-119 and CWE-399 source material. |
| `data/cross_cwe/commit_derived/functions/` | The four newly constructed function-level CWE domains. |
| `data/cross_cwe/commit_derived/evidence/` | Manifests, commit provenance, candidate indexes, and non-silent failure logs for the newly constructed data. |

The distinction between `functions/` and `evidence/` is intentional: files that can be passed to the representation pipeline are separate from records used to audit how those functions were obtained.

## Directory tree

```text
dataset/
├── README.md
├── data/
│   ├── cross_project/
│   │   ├── README.md
│   │   ├── PROJECTS_AND_LICENSES.md
│   │   ├── archives/                 # one ZIP per project
│   │   └── evidence/                 # packaging-only evidence
│   └── cross_cwe/
│       ├── README.md
│       ├── CWE_CATALOG.md
│       ├── public_vuldeepecker/      # CWE-119 and CWE-399
│       └── commit_derived/
│           ├── README.md
│           ├── functions/            # CWE-125/787/416/476 model inputs
│           └── evidence/
│               ├── manifests/
│               └── audit_logs/
```

## Dataset components

### Cross-project domains

Asterisk, FFmpeg, LibPNG, LibTIFF, Pidgin, and VLC are derived from the public Function-level Vulnerability Dataset. Their role, official upstream repositories, application areas, sample counts, and licensing cautions are documented in `data/cross_project/PROJECTS_AND_LICENSES.md`.

### Public cross-CWE domains

CWE-119 and CWE-399 retain the source layout from the public VulDeePecker repository. This component includes C/C++ source, headers, and patch text and is not organized like the four newly constructed function buckets. See `data/cross_cwe/public_vuldeepecker/README.md`.

### Commit-derived cross-CWE domains

CWE-125, CWE-787, CWE-416, and CWE-476 were reconstructed from original FFmpeg and Linux revisions. DiverseVul and Big-Vul were used as commit/CWE indexes rather than as sources of processed function strings. FFmpeg security records supplied additional FFmpeg CVE-to-fix mappings.

| CWE | Vulnerable | Patch-relative non-vulnerable | Membership records |
|---|---:|---:|---:|
| CWE-125 | 175 | 4,529 | 4,704 |
| CWE-787 | 485 | 11,045 | 11,530 |
| CWE-416 | 457 | 12,258 | 12,715 |
| CWE-476 | 270 | 8,180 | 8,450 |
| **Total** | **1,387** | **36,012** | **37,399** |

The 37,399 CWE-membership records correspond to 32,200 globally unique exact source hashes because multi-label membership is retained.

## Label semantics

- `label=1`: the parent-revision function intersects a line deleted or modified by a vulnerability-fixing patch.
- `label=0`: the function comes from the same modified file but does not intersect that patch.

Label 0 therefore means **patch-relative non-vulnerable**, not independently security-audited safe.

## Correct use in transfer experiments

This is a source release, not a universal frozen split. Each directed transfer task must use function-grouped target splitting and pair-specific removal of source/target exact source and final model-input overlaps. Target-test labels must remain unavailable during propagation and representation learning.

## Primary source references

- Function-level Vulnerability Dataset: <https://github.com/Seahymn2019/Function-level-Vulnerability-Dataset>
- VulDeePecker: <https://github.com/CGCL-codes/VulDeePecker>
- DiverseVul: <https://github.com/wagner-group/diversevul>
- Big-Vul: <https://github.com/ZeoVan/MSR_20_Code_vulnerability_CSV_Dataset>
- MITRE CWE: <https://cwe.mitre.org/>


