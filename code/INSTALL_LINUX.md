# CDVD-DPW forpaper — Linux 环境安装指南

整个流水线的依赖分三块：

| 阶段 | 语言/运行时 | 依赖 |
|---|---|---|
| Step 1 切片（轻量版 `gen_slice.py`） | Python 3.10+ | 仅标准库 |
| Step 1 切片（Joern 版 `gen_slice_joern.py`） | Python 3.10+ + JVM | JDK 17+（推荐 21）、Joern |
| Step 2 特征提取（论文后端） | Python 3.8-3.10 | requirements_torch.txt（PyTorch 1.12.0） |
| Step 2 特征提取（兼容后端） | Python 3.10/3.11 | requirements.txt（TensorFlow/gensim） |
| Step 3/4 标签传播 + 度量迁移 | MATLAB R2018a+ | Statistics and Machine Learning Toolbox |

以 Ubuntu 22.04/24.04 为例（Debian 系同理）。

---

## 1. 系统基础包

```bash
sudo apt-get update
sudo apt-get install -y python3 python3-venv python3-pip wget curl unzip
```

## 2. Python 虚拟环境（Step 1 + Step 2）

```bash
cd forpaper
python3 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip
pip install -r requirements_torch.txt
```

版本约束（已在 requirements.txt 固定，勿随意升级）：
- `tensorflow==2.15.1` 自带 keras 2.15，要求 `numpy<2`
- `gensim==4.3.2` 要求 `scipy<1.13`
- GPU 机器：把 `tensorflow==2.15.1` 换成 `tensorflow[and-cuda]==2.15.1`

conda 替代方案：`conda env create -f environment.yml && conda activate cdvd-dpw`

## 3. JDK + Joern（仅 Joern 版切片需要）

```bash
# 3.1 JDK
sudo apt-get install -y openjdk-21-jdk
java -version        # 确认 17+

# 3.2 Joern 2.0.195（论文报告的冻结版本，不使用 latest）
wget -O joern-cli.zip \
  https://github.com/joernio/joern/releases/download/v2.0.195/joern-cli.zip
unzip -t joern-cli.zip                       # 先验证完整性，失败则不要解压
unzip joern-cli.zip -d $HOME/joern
chmod +x $HOME/joern/joern-cli/joern-parse \
         $HOME/joern/joern-cli/joern-export
export PATH="$HOME/joern/joern-cli:$PATH"    # 建议写入 ~/.bashrc

# 3.3 自检（验证 java 与 joern-parse/joern-export 均可用）
cd forpaper/gen_slice
python gen_slice_joern.py --check
```

不想改 PATH 时，可用 `--joern-home $HOME/joern/joern-cli` 或
`export JOERN_HOME=$HOME/joern/joern-cli` 指定位置。

## 4. 运行 Step 1 + Step 2

```bash
cd forpaper
# 论文实验：Joern 精确切片 + 共享源/目标编码器
bash run_step1_step2.sh

# 仅冒烟测试：轻量切片器，结果不可作为论文 Joern 实验报告
USE_LITE=1 bash run_step1_step2.sh
```

单文件切片（如复杂的单个 CVE 函数文件）：

```bash
python gen_slice/gen_slice_joern.py --single CVE-2007-2293.c --label 1 cve.txt
```

## 5. MATLAB（Step 3 + Step 4）

Linux 版 MATLAB R2018a+，勾选 Statistics and Machine Learning Toolbox
（用到 `KDTreeSearcher` / `knnsearch` / `fitctree`）。命令行运行：

```bash
matlab -batch "addpath('Dual-Stage Label Propagation Mechanism'); \
  dslp_main('runs/libpng_to_libtiff/source/30%_labels_features_ext.mat', \
            'runs/libpng_to_libtiff/source/70%_labels_features_ext.mat', \
            'runs/libpng_to_libtiff/extended_training_set.mat')"

matlab -batch "addpath('Dual-Factor Weighted Cross-Domain Representation Learning'); \
  cdr_main('runs/libpng_to_libtiff/extended_training_set.mat', \
           'runs/libpng_to_libtiff/target/5%_labels_features_ext.mat', \
           'runs/libpng_to_libtiff/target/95%_labels_features_ext.mat')"
```

## 常见问题

| 现象 | 处理 |
|---|---|
| `joern-parse not found` | 检查 PATH / `--joern-home` / `$JOERN_HOME` |
| `UnsupportedClassVersionError` | JDK 版本过低，升级到 17+（推荐 21） |
| Joern 内存不足（大文件） | `export JAVA_OPTS="-Xmx8g"` |
| pip 装 scipy 报编译错误 | 确认 Python 为 3.10/3.11（3.12 需调整版本组合） |
| 改了切片文件但旧兼容入口结果没变 | 删除 `Feature_Representation_Model/*_gadget_vectors_v2.pkl` 缓存 |
