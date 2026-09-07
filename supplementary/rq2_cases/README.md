# RQ2 function-level case study

This directory provides four function-level cases used to explain the behavior of CDVD-DPW in cross-project vulnerability detection. The cases cover a true positive, a true negative, a false positive, and a false negative, including both directions of the difficult Pidgin--Asterisk transfer setting.

The experimental split is defined at the function level, whereas CDVD-DPW operates on 128-dimensional representations of vulnerability-oriented slices. Each case therefore contains the complete target function together with its slice-level evidence and a representative source-domain function used in the cross-domain interpretation.

## Directory layout

```text
rq2_cases/
├── README.md
├── CASE_STUDY.md
├── case_manifest.csv
├── SHA256SUMS.txt
├── TP/
├── TN/
├── FP/
└── FN/
```

Each outcome directory contains:

- `target_function.c`: the complete target-domain function;
- `source_evidence.c`: the source-domain function discussed in the case analysis;
- `evidence/target_slices.cgd.txt`: all unique target slice records associated with the function;
- `evidence/target_manifest_rows.csv`: model-input hashes, candidate types, labels, and duplicate counts;
- `evidence/target_split_rows.csv`: frozen target-test membership;
- `evidence/source_slices.cgd.txt`: the corresponding source-domain slice records.

## Case index

| Case | Direction | Target project/function | Ground truth | Target split | Unique target slices |
|---|---|---|---|---|---:|
| TP | LibPNG→LibTIFF | LibTIFF `t2p_process_jpeg_strip` | Vulnerable | U95 test | 35 |
| TN | LibPNG→LibTIFF | LibTIFF `checkAdd64` | Non-vulnerable | U95 test | 2 |
| FP | Pidgin→Asterisk | Asterisk `gen_prios` | Non-vulnerable | U95 test | 402 |
| FN | Asterisk→Pidgin | Pidgin `mxit_parse_packet` | Vulnerable | U95 test | 19 |

`case_manifest.csv` provides the source hashes and processed-data statistics used to trace every case. `SHA256SUMS.txt` covers all files in this directory except the checksum file itself.

The examples explain known vulnerability-detection outcomes; they do not claim discovery of previously unknown vulnerabilities.
