#!/usr/bin/env bash
# run_step1_step2.sh -- Linux one-shot runner for Step 1 (program slicing)
# and Step 2 (BiLSTM feature extraction) of the CDVD-DPW forpaper pipeline.
#
# Usage (from the forpaper/ directory):
#   bash run_step1_step2.sh
#
# Prerequisites: python3 (>=3.10) with venv; see requirements.txt.
# Outputs land in runs/libpng_to_libtiff/ as the
# .mat files consumed by the MATLAB modules (Step 3/4).

set -euo pipefail
cd "$(dirname "$0")"

# ---- 0. Environment ------------------------------------------------------
if [ ! -d .venv ]; then
    python3 -m venv .venv
fi
source .venv/bin/activate
BACKEND="${BACKEND:-torch}"
if [ "$BACKEND" = "torch" ]; then
    python -c 'import sys; assert sys.version_info[:2] <= (3,10), "PyTorch 1.12 paper backend requires Python <=3.10"'
    pip install -r requirements_torch.txt
else
    pip install -r requirements.txt
fi

# ---- 1. Step 1: program slicing (paper 3.3.1-3.3.2) ----------------------
# Source domain: LibPNG (30% labels)  Target domain: LibTIFF (5% labels)
# Joern is mandatory for reported experiments. USE_LITE=1 is only a local
# smoke test; its outputs must not be reported as Joern/PDG results.
cd gen_slice
if [ "${USE_LITE:-0}" = "1" ]; then
    python gen_slice.py ../LibPNG  libpng_cgd.txt
    python gen_slice.py ../LibTIFF libtiff_cgd.txt
else
    python gen_slice_joern.py --check
    python gen_slice_joern.py ../LibPNG  libpng_cgd.txt
    python gen_slice_joern.py ../LibTIFF libtiff_cgd.txt
fi
cd ..

# ---- 2. Step 2: BiLSTM feature representation (paper 3.3.3) --------------
cd Feature_Representation_Model
python extract_transfer_features.py \
    ../gen_slice/libpng_cgd.txt ../gen_slice/libtiff_cgd.txt \
    ../runs/libpng_to_libtiff \
    --source-functions ../LibPNG --target-functions ../LibTIFF \
    --source-label-fraction 0.30 --target-label-fraction 0.05 --seed 42 \
    --backend "$BACKEND"
cd ..

echo
echo "Done. Feature .mat files:"
find runs/libpng_to_libtiff -name '*_labels_features_ext.mat'
