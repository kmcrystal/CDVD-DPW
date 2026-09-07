#!/usr/bin/env python3
"""
gen_slice_joern.py -- Joern-backed Data Preprocessing Module of CDVD-DPW
(paper Section 3.3; drop-in alternative to gen_slice.py)

Uses Joern (https://joern.io) to build precise Program Dependence Graphs
with a real C/C++ frontend (c2cpg), instead of the lightweight line-level
heuristics of gen_slice.py. Recommended for long/complex functions with
deep nesting, multi-line statements, and heavy pointer/struct usage.

Per .c file:
  1. `joern-parse`  builds the Code Property Graph (CPG)
  2. `joern-export --repr pdg` dumps one DOT graph per method
     (nodes carry code text + line numbers; edges are DDG/CDG)
  3. candidate suspicious points (paper 3.3.1) are located on CPG nodes:
       FC: calls to risky library APIs
       AU: array usage            (<operator>.indexAccess / indirectIndexAccess)
       PU: pointer operations     (<operator>.indirection / addressOf /
                                   indirectFieldAccess / fieldAccess on ->)
       AE: arithmetic expressions (<operator>.addition / subtraction / ...)
  4. forward + backward BFS over the PDG from each candidate node yields
     the program slice (paper 3.3.2), emitted in the same cgd format as
     gen_slice.py:

        <index> <source-file> <candidate-type> <candidate-line> <candidate-slice-pos>
        <slice lines...>
        <label 0|1>
        ---------------------------------

Usage:
    python gen_slice_joern.py <project_dir> <output_cgd_file> [--joern-home DIR]
    python gen_slice_joern.py --single file.c --label 1 out.txt [--joern-home DIR]
    python gen_slice_joern.py --check           # verify java/joern setup

<project_dir> must contain Vulnerable_functions/ (label 1) and
Non_vulnerable_functions/ (label 0), like forpaper/LibPNG.

Requirements (Linux): JDK 17+ (21 recommended) and Joern; joern-parse /
joern-export found via --joern-home, $JOERN_HOME, or $PATH.
"""

import argparse
import html
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

EXPECTED_JOERN_VERSION = "2.0.195"

# ---------------------------------------------------------------------------
# Candidate classification (paper 3.3.1)
# ---------------------------------------------------------------------------

RISKY_APIS = {
    "strcpy", "strncpy", "strcat", "strncat", "sprintf", "snprintf",
    "vsprintf", "vsnprintf", "gets", "fgets", "scanf", "sscanf", "fscanf",
    "memcpy", "memmove", "memset", "memcmp", "bcopy", "alloca",
    "malloc", "calloc", "realloc", "free",
    "read", "fread", "write", "fwrite", "recv", "recvfrom",
    "getenv", "system", "popen", "exec", "strlen", "strtok", "atoi", "atol",
    "strdup", "strdupa", "strcasecmp", "htons", "ntohs",
}

ARRAY_OPS = {
    "<operator>.indexAccess", "<operator>.indirectIndexAccess",
}
POINTER_OPS = {
    "<operator>.indirection", "<operator>.addressOf",
    "<operator>.indirectFieldAccess", "<operator>.pointerShift",
}
ARITH_OPS = {
    "<operator>.addition", "<operator>.subtraction",
    "<operator>.multiplication", "<operator>.division",
    "<operator>.modulo", "<operator>.shiftLeft",
    "<operator>.arithmeticShiftRight", "<operator>.logicalShiftRight",
    "<operator>.preIncrement", "<operator>.postIncrement",
    "<operator>.preDecrement", "<operator>.postDecrement",
}

# priority FC > AU > PU > AE (same as gen_slice.py)
PRIORITY = {"FC": 0, "AU": 1, "PU": 2, "AE": 3}


def classify_node(name, code):
    """Map a PDG node to a candidate class, or None."""
    if name in ARRAY_OPS:
        return "AU"
    if name in POINTER_OPS:
        return "PU"
    if name in ARITH_OPS:
        return "AE"
    if name and not name.startswith("<operator>"):
        base = name.strip()
        if base in RISKY_APIS:
            return "FC:" + base
    # fallback: textual classification of the node's code (robust to
    # differences in export label conventions across joern versions)
    if re.search(r"\b(" + "|".join(sorted(RISKY_APIS)) + r")\s*\(", code):
        m = re.search(r"\b(" + "|".join(sorted(RISKY_APIS)) + r")\s*\(", code)
        return "FC:" + m.group(1)
    return None


# ---------------------------------------------------------------------------
# Joern invocation
# ---------------------------------------------------------------------------

def find_joern(joern_home):
    """Locate joern-parse / joern-export."""
    candidates = []
    if joern_home:
        candidates.append(joern_home)
    if os.environ.get("JOERN_HOME"):
        candidates.append(os.environ["JOERN_HOME"])
    for base in candidates:
        parse = os.path.join(base, "joern-parse")
        export = os.path.join(base, "joern-export")
        if os.path.isfile(parse) and os.path.isfile(export):
            return parse, export
    parse = shutil.which("joern-parse")
    export = shutil.which("joern-export")
    if parse and export:
        return parse, export
    sys.exit("error: joern-parse/joern-export not found. Install Joern and "
             "pass --joern-home, set $JOERN_HOME, or add it to $PATH.")


def run_quiet(cmd, cwd):
    proc = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT, text=True)
    if proc.returncode != 0:
        raise RuntimeError("command failed: %s\n%s" % (" ".join(cmd),
                                                       proc.stdout[-2000:]))


def export_pdg(src_file, joern_parse, joern_export, workdir):
    """Parse one .c file and export its PDG DOT graphs; returns dot dir."""
    cpg = os.path.join(workdir, "cpg.bin")
    outdir = os.path.join(workdir, "pdg")     # must not pre-exist
    run_quiet([joern_parse, os.path.abspath(src_file), "--output", cpg],
              cwd=workdir)
    run_quiet([joern_export, cpg, "--repr", "pdg", "--out", outdir],
              cwd=workdir)
    return outdir


# ---------------------------------------------------------------------------
# DOT parsing
# ---------------------------------------------------------------------------

# "123" [label = <(CALL,memcpy(&amp;sin.sin_addr, ...))<SUB>114</SUB>> ]
NODE_RE = re.compile(
    r'^\s*"?(\d+)"?\s*\[\s*label\s*=\s*<\((.*)\)(?:<SUB>(\d+)</SUB>)?>\s*\]')
# "123" -> "456" [ label = "DDG: portno" ]
EDGE_RE = re.compile(r'^\s*"?(\d+)"?\s*->\s*"?(\d+)"?')


def parse_dot(path):
    """Return (nodes {id: (name, code, line)}, fwd {id: set}, bwd {id: set})."""
    nodes, fwd, bwd = {}, {}, {}
    with open(path, "r", encoding="utf8", errors="ignore") as fh:
        for raw in fh:
            m = NODE_RE.match(raw)
            if m:
                nid = m.group(1)
                body = html.unescape(m.group(2))
                line = int(m.group(3)) if m.group(3) else None
                name, _, code = body.partition(",")
                nodes[nid] = (name.strip(), code.strip(), line)
                continue
            m = EDGE_RE.match(raw)
            if m:
                a, b = m.group(1), m.group(2)
                fwd.setdefault(a, set()).add(b)
                bwd.setdefault(b, set()).add(a)
    return nodes, fwd, bwd


def bfs_lines(adj, nodes, start):
    """BFS over one edge direction; collect line numbers of visited nodes."""
    seen, queue, lines = {start}, [start], set()
    while queue:
        cur = queue.pop(0)
        line = nodes.get(cur, (None, None, None))[2]
        if line is not None:
            lines.add(line)
        for nxt in adj.get(cur, ()):
            if nxt not in seen:
                seen.add(nxt)
                queue.append(nxt)
    return lines


# ---------------------------------------------------------------------------
# Slicing one source file
# ---------------------------------------------------------------------------

def slice_file(src_file, label, out, index, joern_parse, joern_export):
    with open(src_file, "r", encoding="utf8", errors="ignore") as fh:
        src_lines = fh.read().split("\n")

    workdir = tempfile.mkdtemp(prefix="joern_slice_")
    try:
        dot_dir = export_pdg(src_file, joern_parse, joern_export, workdir)

        # candidate collection across all method PDGs of the file:
        # line -> (priority-best ctype, node id, graph)
        best = {}
        graphs = []
        for dot in sorted(os.listdir(dot_dir)):
            if not dot.endswith(".dot"):
                continue
            nodes, fwd, bwd = parse_dot(os.path.join(dot_dir, dot))
            graphs.append((nodes, fwd, bwd))
            gidx = len(graphs) - 1
            for nid, (name, code, line) in nodes.items():
                if line is None:
                    continue
                ctype = classify_node(name, code)
                if ctype is None:
                    continue
                key = PRIORITY[ctype[:2]]
                if line not in best or key < PRIORITY[best[line][0][:2]]:
                    best[line] = (ctype, nid, gidx)

        name = os.path.basename(src_file)
        for cand_line in sorted(best):
            ctype, nid, gidx = best[cand_line]
            nodes, fwd, bwd = graphs[gidx]
            lines = bfs_lines(fwd, nodes, nid) | bfs_lines(bwd, nodes, nid)
            lines = sorted(l for l in lines if 1 <= l <= len(src_lines))

            sliced, cand_pos = [], 0
            for l in lines:
                text = src_lines[l - 1].strip()
                if not text or text.startswith("#"):
                    continue
                if l == cand_line:
                    cand_pos = len(sliced)
                sliced.append(text)
            if len(sliced) < 3:
                continue
            out.write("%d %s %s %d %d\n" %
                      (index, name, ctype, cand_line, cand_pos))
            out.write("\n".join(sliced) + "\n")
            out.write("%d\n" % label)
            out.write("-" * 33 + "\n")
            index += 1
    finally:
        shutil.rmtree(workdir, ignore_errors=True)
    return index


# ---------------------------------------------------------------------------
# Driver
# ---------------------------------------------------------------------------

def check_environment(joern_home):
    java = shutil.which("java")
    if not java:
        sys.exit("error: java not found (need JDK 17+, e.g. "
                 "`sudo apt install openjdk-21-jdk`)")
    ver = subprocess.run(["java", "-version"], capture_output=True, text=True)
    print("java:", (ver.stderr or ver.stdout).splitlines()[0])
    parse, export = find_joern(joern_home)
    print("joern-parse:", parse)
    print("joern-export:", export)
    version = subprocess.run([parse, "--version"], capture_output=True,
                             text=True)
    version_text = (version.stdout or version.stderr).strip().splitlines()
    version_text = version_text[0] if version_text else "unknown"
    print("joern version:", version_text)
    if EXPECTED_JOERN_VERSION not in version_text:
        print("warning: paper uses Joern %s; detected %s" %
              (EXPECTED_JOERN_VERSION, version_text))
    print("environment OK")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("project_dir", nargs="?",
                    help="dir with Vulnerable_functions/ and "
                         "Non_vulnerable_functions/")
    ap.add_argument("output", nargs="?", help="output cgd file")
    ap.add_argument("--single", help="slice a single .c file instead")
    ap.add_argument("--label", type=int, default=1,
                    help="label for --single mode (default 1)")
    ap.add_argument("--joern-home", help="Joern installation directory")
    ap.add_argument("--check", action="store_true",
                    help="verify java/joern setup and exit")
    ap.add_argument("--failures-log",
                    help="JSONL failure audit (default: <output>.failures.jsonl)")
    args = ap.parse_args()

    if args.check:
        check_environment(args.joern_home)
        return

    joern_parse, joern_export = find_joern(args.joern_home)
    index = 0

    if args.single:
        out_path = args.output or args.project_dir
        if not out_path:
            ap.error("--single requires an output file")
        with open(out_path, "w", encoding="utf8") as out:
            index = slice_file(args.single, args.label, out, index,
                               joern_parse, joern_export)
        print("wrote %d slices to %s" % (index, out_path))
        return

    if not args.project_dir or not args.output:
        ap.error("project_dir and output are required (or use --single)")

    groups = [("Vulnerable_functions", 1), ("Non_vulnerable_functions", 0)]
    failures = []
    with open(args.output, "w", encoding="utf8") as out:
        for folder, label in groups:
            full = os.path.join(args.project_dir, folder)
            if not os.path.isdir(full):
                print("warning: missing folder %s" % full)
                continue
            files = sorted(f for f in os.listdir(full) if f.endswith(".c"))
            for i, fname in enumerate(files):
                print("[%s %d/%d] %s" % (folder, i + 1, len(files), fname))
                try:
                    index = slice_file(os.path.join(full, fname), label,
                                       out, index, joern_parse, joern_export)
                except RuntimeError as e:
                    print("  skip (%s)" % str(e).splitlines()[0])
                    failures.append({"folder": folder, "label": label,
                                     "file": fname, "error": str(e)})
    print("wrote %d slices to %s" % (index, args.output))
    failure_path = args.failures_log or (args.output + ".failures.jsonl")
    with open(failure_path, "w", encoding="utf8") as audit:
        for failure in failures:
            audit.write(json.dumps(failure, ensure_ascii=False) + "\n")
    print("skipped %d files; failure audit: %s" % (len(failures), failure_path))


if __name__ == "__main__":
    main()
