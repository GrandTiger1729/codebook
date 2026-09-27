#!/usr/bin/env python3
"""Fail if any listing in the PDF would wrap.

listings at \\footnotesize in the two-column layout fits 58 columns (tab = 2);
column 59 on is pushed onto a second line. Checks every file content.tex
pulls in with \\inputcode or \\lstinputlisting (commented-out lines skipped).

    python3 verify/check_layout.py        # exit 1 and list offenders if any
"""
import re
import sys
from pathlib import Path

LIMIT = 58
BOOK = Path(__file__).resolve().parent.parent / "codebook"
INPUT = re.compile(r"\\(?:inputcode\{[^}]*\}|lstinputlisting(?:\[[^]]*\])?)\{([^}]+)\}")


def width(line: str) -> int:
    col = 0
    for ch in line:
        col = (col // 2 + 1) * 2 if ch == "\t" else col + 1
    return col


bad = 0
for tex in (BOOK / "content.tex").read_text(encoding="utf-8").splitlines():
    if tex.lstrip().startswith("%"):
        continue
    for name in INPUT.findall(tex.split("%")[0]):
        text = (BOOK / name).read_text(encoding="utf-8", errors="replace")
        for no, line in enumerate(text.split("\n"), 1):
            if (w := width(line.rstrip("\r"))) > LIMIT:
                print(f"codebook/{name}:{no}: {w} columns: {line.strip()}")
                bad += 1
print(f"{bad} line(s) over {LIMIT} columns" if bad else f"all listings fit in {LIMIT} columns")
sys.exit(bad > 0)
