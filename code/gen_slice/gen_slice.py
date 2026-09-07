"""
gen_slice.py -- Data Preprocessing Module of CDVD-DPW (paper Section 3.3)

Transforms C source functions into candidate-point-centered program slices:

  1. Candidate suspicious point identification (Section 3.3.1)
     Four syntactic feature classes are extracted as candidate points:
       - FC: API function calls           (may introduce security flaws)
       - AU: array usage                  (buffer-related issues)
       - PU: pointer operations           (memory access errors)
       - AE: arithmetic expressions       (overflow anomalies)

  2. PDG construction (Section 3.3.1)
     For each function a Program Dependence Graph G = (V, E) is built at
     statement (line) granularity with two edge types:
       - data dependency:    line j uses a variable defined at line i
       - control dependency: line j executes under the condition at line i

  3. Candidate-point-centered slicing (Section 3.3.2)
     For each candidate point s_ij the forward slice fs_ij (all PDG nodes
     reachable from s_ij) and backward slice bs_ij (all PDG nodes that can
     reach s_ij) are merged into the program slice ps_ij.

Output is a "code gadget" (cgd) file in the VulDeePecker-compatible format
consumed by the Feature_Representation_Model module:

    <index> <source-file> <candidate-type> <candidate-line> <candidate-slice-pos>
    <slice line 1>
    ...
    <slice line n>
    <label 0|1>
    ---------------------------------

<candidate-slice-pos> is the 0-based index of the candidate statement inside
the slice; Feature_Representation_Model uses it to center the 500-token
sequence on the candidate point (Section 3.3.2).

Usage:
    python gen_slice.py <project_dir> <output_cgd_file>

    <project_dir> must contain the two sub-folders
        Vulnerable_functions/      (*.c, label 1)
        Non_vulnerable_functions/  (*.c, label 0)

Example:
    python gen_slice.py ../LibPNG libpng_cgd.txt
    python gen_slice.py ../LibTIFF libtiff_cgd.txt

Note: the full project uses Joern to build inter-procedural PDGs from whole
code bases. The forpaper data ships function-level samples, so this module
implements the intra-procedural PDG construction described in the paper
directly, with no external dependency.
"""

import os
import re
import sys

# ----------------------------------------------------------------------------
# Candidate suspicious point identification (Section 3.3.1)
# ----------------------------------------------------------------------------

# C library / API calls with documented correlation to security defects.
RISKY_APIS = {
    "strcpy", "strncpy", "strcat", "strncat", "sprintf", "snprintf",
    "vsprintf", "vsnprintf", "gets", "fgets", "scanf", "sscanf", "fscanf",
    "memcpy", "memmove", "memset", "memcmp", "bcopy", "alloca",
    "malloc", "calloc", "realloc", "free",
    "read", "fread", "write", "fwrite", "recv", "recvfrom",
    "getenv", "system", "popen", "exec", "strlen", "strtok", "atoi", "atol",
}

C_KEYWORDS = {
    "if", "else", "for", "while", "do", "switch", "case", "default",
    "return", "break", "continue", "goto", "sizeof", "typedef", "struct",
    "union", "enum", "static", "const", "unsigned", "signed", "void",
    "int", "char", "long", "short", "float", "double", "defined",
}

RE_CALL = re.compile(r"\b([A-Za-z_]\w*)\s*\(")
RE_ARRAY = re.compile(r"\b[A-Za-z_]\w*\s*\[")
RE_POINTER = re.compile(r"(\*\s*[A-Za-z_]\w*|\b[A-Za-z_]\w*\s*->|&\s*[A-Za-z_]\w*)")
RE_ARITH = re.compile(r"[A-Za-z_0-9\)\]]\s*(\+|\-|\*|/|%|<<|>>)\s*[A-Za-z_0-9\(]")
RE_IDENT = re.compile(r"\b[A-Za-z_]\w*\b")
RE_CONTROL = re.compile(r"^\s*(if|for|while|switch|else\s+if)\b")
RE_ASSIGN = re.compile(
    r"([A-Za-z_]\w*(?:\s*(?:\[[^\]]*\]|->\s*\w+|\.\s*\w+))*)\s*"
    r"(=|\+=|-=|\*=|/=|%=|&=|\|=|\^=|<<=|>>=|\+\+|--)"
)
RE_DECL = re.compile(
    r"^\s*(?:static\s+|const\s+|unsigned\s+|signed\s+|struct\s+\w+\s*|"
    r"[A-Za-z_]\w*\s+)+[\*\s]*([A-Za-z_]\w*(?:\s*,\s*\**[A-Za-z_]\w*)*)\s*[=;,\[]"
)


def strip_comments(code):
    """Remove /* */ and // comments plus non-ASCII characters (Section 3.3.2)."""
    code = re.sub(r"/\*.*?\*/", lambda m: re.sub(r"[^\n]", " ", m.group(0)),
                  code, flags=re.S)
    code = re.sub(r"//[^\n]*", "", code)
    code = code.encode("ascii", errors="ignore").decode("ascii")
    return code


def find_candidate_points(lines):
    """Return [(line_no, type)] for the four candidate classes FC/AU/PU/AE."""
    candidates = []
    for no, line in enumerate(lines, start=1):
        stripped = line.strip()
        if not stripped or stripped.startswith("#"):
            continue
        # FC: API/library function calls
        for m in RE_CALL.finditer(line):
            name = m.group(1)
            if name in C_KEYWORDS:
                continue
            if name in RISKY_APIS:
                candidates.append((no, "FC:" + name))
                break
        # AU: array usage
        if RE_ARRAY.search(line):
            candidates.append((no, "AU"))
        # PU: pointer operations
        elif RE_POINTER.search(line):
            candidates.append((no, "PU"))
        # AE: arithmetic expressions
        elif RE_ARITH.search(line):
            candidates.append((no, "AE"))
    # Deduplicate per line, keep the highest-priority class (FC > AU > PU > AE)
    best = {}
    priority = {"F": 0, "A": 1, "P": 2, "E": 3}
    for no, ctype in candidates:
        if no not in best or priority[ctype[0]] < priority[best[no][0]]:
            best[no] = ctype
    return sorted(best.items())


# ----------------------------------------------------------------------------
# PDG construction (Section 3.3.1)
# ----------------------------------------------------------------------------

def variables_of(line):
    """Identifiers appearing on a line, excluding keywords and literals."""
    return {t for t in RE_IDENT.findall(line)
            if t not in C_KEYWORDS and not t.isdigit()}


def defined_variables(line):
    """Variables (re)defined on this line: assignments and declarations."""
    defs = set()
    for m in RE_ASSIGN.finditer(line):
        base = re.match(r"[A-Za-z_]\w*", m.group(1))
        if base:
            defs.add(base.group(0))
    m = RE_DECL.match(line)
    if m:
        for name in re.split(r"\s*,\s*", m.group(1)):
            base = re.match(r"\**\s*([A-Za-z_]\w*)", name.strip())
            if base:
                defs.add(base.group(1))
    return defs


def build_pdg(lines):
    """
    Build the statement-level PDG.
    Returns (forward_adj, backward_adj): dict line_no -> set(line_no).
    """
    n = len(lines)
    forward = {i: set() for i in range(1, n + 1)}
    backward = {i: set() for i in range(1, n + 1)}

    def add_edge(src, dst):
        if src != dst:
            forward[src].add(dst)
            backward[dst].add(src)

    # --- data dependencies: def at line i -> use at line j (reaching defs) ---
    last_def = {}                       # variable -> most recent defining line
    for no in range(1, n + 1):
        line = lines[no - 1]
        uses = variables_of(line)
        defs = defined_variables(line)
        for var in uses:
            if var in last_def and (var not in defs or no != last_def[var]):
                add_edge(last_def[var], no)
        for var in defs:
            last_def[var] = no

    # --- control dependencies: condition line -> lines in its block ---------
    # Track an open-brace stack; a control statement dominates the statements
    # inside the block it opens (single-statement bodies handled as next line).
    stack = []                          # (control_line_no, depth_when_opened)
    depth = 0
    pending_control = None              # control line waiting for '{' or body
    for no in range(1, n + 1):
        line = lines[no - 1]
        stripped = line.strip()

        for ctrl, _ in stack:
            add_edge(ctrl, no)

        if pending_control is not None and pending_control != no:
            if "{" not in lines[pending_control - 1]:
                if stripped and not stripped.startswith("{"):
                    # single-statement body
                    add_edge(pending_control, no)
                    pending_control = None
                elif stripped.startswith("{"):
                    stack.append((pending_control, depth))
                    pending_control = None

        if RE_CONTROL.match(line):
            if "{" in line:
                stack.append((no, depth))
            else:
                pending_control = no

        depth += line.count("{") - line.count("}")
        while stack and depth <= stack[-1][1]:
            stack.pop()

    return forward, backward


# ----------------------------------------------------------------------------
# Candidate-point-centered slicing (Section 3.3.2)
# ----------------------------------------------------------------------------

def reachable(adj, start):
    """BFS over one edge direction; returns the set of reached line numbers."""
    seen = {start}
    queue = [start]
    while queue:
        node = queue.pop(0)
        for nxt in adj.get(node, ()):  # noqa: B905
            if nxt not in seen:
                seen.add(nxt)
                queue.append(nxt)
    return seen


def slice_function(lines, candidate_line):
    """
    ps = forward slice ∪ backward slice, in original line order.
    Returns (slice_lines, candidate_pos) where candidate_pos is the index of
    the candidate statement within slice_lines (used for token centering).
    """
    forward, backward = build_pdg(lines)
    fs = reachable(forward, candidate_line)
    bs = reachable(backward, candidate_line)
    keep = sorted(fs | bs)

    sliced, candidate_pos = [], 0
    for no in keep:
        text = lines[no - 1].strip()
        if not text or text.startswith("#"):
            continue
        if no == candidate_line:
            candidate_pos = len(sliced)
        sliced.append(text)
    return sliced, candidate_pos


# ----------------------------------------------------------------------------
# Driver
# ----------------------------------------------------------------------------

def process_file(path, label, out, index):
    with open(path, "r", encoding="utf8", errors="ignore") as fh:
        code = strip_comments(fh.read())
    lines = code.split("\n")

    candidates = find_candidate_points(lines)
    if not candidates:
        return index

    name = os.path.basename(path)
    for cand_line, ctype in candidates:
        sliced, cand_pos = slice_function(lines, cand_line)
        if len(sliced) < 3:            # discard trivial slices
            continue
        out.write("%d %s %s %d %d\n" % (index, name, ctype, cand_line, cand_pos))
        out.write("\n".join(sliced) + "\n")
        out.write("%d\n" % label)
        out.write("-" * 33 + "\n")
        index += 1
    return index


def main():
    if len(sys.argv) != 3:
        print(__doc__)
        sys.exit(1)
    project_dir, out_path = sys.argv[1], sys.argv[2]

    groups = [("Vulnerable_functions", 1), ("Non_vulnerable_functions", 0)]
    index = 0
    with open(out_path, "w", encoding="utf8") as out:
        for folder, label in groups:
            full = os.path.join(project_dir, folder)
            if not os.path.isdir(full):
                print("warning: missing folder %s" % full)
                continue
            for fname in sorted(os.listdir(full)):
                if fname.endswith(".c"):
                    index = process_file(os.path.join(full, fname),
                                         label, out, index)
    print("wrote %d slices to %s" % (index, out_path))


if __name__ == "__main__":
    main()
