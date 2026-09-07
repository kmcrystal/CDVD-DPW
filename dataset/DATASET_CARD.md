# Dataset Card

## Dataset summary

This dataset is designed to evaluate vulnerability detection under limited labels and domain shift. Projects define domains in cross-project evaluation; CWE categories define domains in cross-type evaluation. The release preserves function source code, labels, and construction provenance so that researchers can rebuild leakage-controlled task splits instead of relying on opaque preprocessed tensors.

## Intended uses

- cross-project and cross-CWE vulnerability detection;
- semi-supervised and domain-adaptation evaluation under explicitly limited labels;
- representation, slicing, and duplicate-sensitivity studies;
- audits of patch-derived function labeling.

## Uses outside scope

- treating every `label=0` function as independently security-audited safe;
- using this archive as a benchmark without source/target duplicate control;
- compiling or executing the vulnerable functions as standalone programs;
- estimating real-world vulnerability prevalence from the class ratios;
- claiming CVE-level detection when evaluation is performed at function or slice level.

## Composition

### Cross-project component

| Project | Vulnerable functions | Non-vulnerable functions | Total files in archive |
|---|---:|---:|---:|
| Asterisk | 56 | 18,206 | 18,262 |
| FFmpeg | 213 | 5,552 | 5,765 |
| LibPNG | 45 | 577 | 622 |
| LibTIFF | 96 | 731 | 827 |
| Pidgin | 29 | 8,628 | 8,657 |
| VLC | 44 | 6,115 | 6,159 |

Counts above are inventories of the staged ZIP files, not post-slicing sample counts.

### Commit-derived cross-CWE component

| CWE | FFmpeg vulnerable | Linux vulnerable | Vulnerable total | Patch-relative non-vulnerable | Total |
|---|---:|---:|---:|---:|---:|
| CWE-125 | 67 | 108 | 175 | 4,529 | 4,704 |
| CWE-787 | 82 | 403 | 485 | 11,045 | 11,530 |
| CWE-416 | 46 | 411 | 457 | 12,258 | 12,715 |
| CWE-476 | 49 | 221 | 270 | 8,180 | 8,450 |

The non-vulnerable counts by project are available in `data/cross_cwe/commit_derived/evidence/manifests/expanded_manifest.csv`.

### Legacy CWE-119 and CWE-399 material

The `CWE-119/source_files` and `CWE-399/source_files` trees retain the source layout obtained from the VulDeePecker data repository. They include C/C++ source, headers, and patch text and are structurally different from the four commit-derived function buckets. They should be processed through the stated representation pipeline before task construction; raw file counts are not directly comparable with the function-record counts above.

## Collection and labeling

Candidate CWE/repository/fix-commit tuples came from DiverseVul and Big-Vul metadata. The processed function strings distributed by those datasets were not used. For each selected fix, the original repository was queried, the first parent of the fix was used as the pre-fix state, and parent-side changed lines were mapped to enclosing complete C functions. Additional FFmpeg mappings came from FFmpeg security records.

Functions intersecting deleted or modified parent-side lines received `label=1`. Other complete functions in the same modified C files received `label=0` under the patch-relative semantics described above.

## Deduplication and conflicts

Source text is addressed by lowercase SHA-256. Within each CWE/label bucket, each hash appears once. Multi-label membership is preserved, so identical text can appear in multiple CWE buckets.

Across the full raw provenance table, 197 hashes had both label observations. The construction rule is conservative: if a source hash was vulnerable in any retained context, its released label is vulnerable. In the four target CWE buckets, 260 membership rows are marked `label_conflict_promoted=1`. The final release has no hash simultaneously stored under vulnerable and non-vulnerable directories.

The promotion rule resolves contradictory function-level labels; it does not prove that every slice inside a vulnerable function is vulnerability-relevant.

## Known biases and limitations

- Patch-derived labels are weak function-level labels and inherit patch incompleteness or CWE noise.
- Non-vulnerable examples come only from files touched by selected fixes and are not a random sample of all project code.
- Linux dominates the four expanded CWE domains, particularly CWE-416 and CWE-787.
- The dataset is highly class-imbalanced by design; no balancing or sampling was applied during construction.
- Multiple commits may touch the same function text, and one function may carry multiple CWE memberships.
- Type-3 and semantic clones are not guaranteed to be removed by exact source hashing.
- End-to-end reconstruction also depends on the exact function-boundary parser and extraction program; see `docs/release/RELEASE_CHECKLIST.md`.

## Security and ethics

The archive contains real vulnerable source fragments. It is intended for defensive research and should be handled as untrusted text. Do not compile or execute files from the archive without isolation. CVE identifiers and public commit links are retained for scientific traceability; no private user data is included.
