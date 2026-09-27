#!/usr/bin/env python3
"""Run codebook templates against Library Checker test data.

Usage:
    uv run run.py --all
    uv run run.py -p zalgorithm -p scc
"""
import argparse
import concurrent.futures as cf
import os
import shutil
import subprocess
import sys
import threading
import time
import tomllib
import resource
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
LCP = HERE / "library-checker-problems"  # git submodule
CODEBOOK = HERE.parent / "codebook"
BUILD = HERE / "build"
PY = HERE / ".venv" / "bin" / "python"

SLOW_RATIO = 0.5  # max_time / timelimit above this gets flagged


@dataclass
class Row:
    problem: str          # "graph/scc", or "graph/scc#tag" when two rows share a problem
    driver: str
    templates: str
    notes: str = ""

    @property
    def dir(self) -> str:
        return self.problem.split("#", 1)[0]


@dataclass
class Result:
    row: Row
    verdict: str = "?"
    detail: str = ""
    cases: int = 0
    max_time: float = 0.0
    max_case: str = ""
    timelimit: float = 0.0
    failures: list = field(default_factory=list)

    @property
    def ratio(self):
        return self.max_time / self.timelimit if self.timelimit else 0.0


def read_map(path: Path) -> list[Row]:
    rows = []
    for line in path.read_text().splitlines():
        line = line.rstrip()
        if not line or line.lstrip().startswith("#"):
            continue
        parts = line.split("\t")
        while len(parts) < 4:
            parts.append("")
        rows.append(Row(*[p.strip() for p in parts[:4]]))
    return rows


_gen_locks: dict[str, threading.Lock] = {}
_gen_locks_guard = threading.Lock()


def generate(problem: str, log) -> bool:
    # Several MAP rows share a problem (ordered_set, ordered_set#treap, ...).
    # Two generate.py runs on one directory race: one is still writing the
    # checker binary while the other execs it (EACCES on a fresh checkout).
    with _gen_locks_guard:
        lock = _gen_locks.setdefault(problem, threading.Lock())
    with lock:
        return _generate(problem, log)


def _generate(problem: str, log) -> bool:
    info = LCP / problem / "info.toml"
    if not info.exists():
        log(f"  no info.toml at {problem}")
        return False
    # LC's default flags carry -Werror, and GCC 16 warns on some reference
    # solutions; drop it so test data still generates.
    env = dict(os.environ,
               CXXFLAGS="-O2 -std=c++17 -Wall -Wextra -Wno-unused-result")
    p = subprocess.run(
        [str(PY), "generate.py", str(info)],
        cwd=LCP, capture_output=True, text=True, env=env,
    )
    if p.returncode != 0:
        log("  generate.py failed:\n" + (p.stderr or p.stdout)[-1500:])
        return False
    return True


def compile_driver(row: Row, log) -> Path | None:
    # the driver column may carry extra compiler flags: "fps.cpp -DOP_INV"
    name, *flags = row.driver.split()
    src = HERE / "drivers" / name
    if not src.exists():
        log(f"  missing driver {src}")
        return None
    BUILD.mkdir(exist_ok=True)
    tag = "".join(c if c.isalnum() else "_" for c in "".join(flags))
    exe = BUILD / (Path(name).stem + tag + "")
    cmd = ["g++", "-std=c++20", "-O2", "-w", *flags, "-o", str(exe), str(src)]
    p = subprocess.run(cmd, capture_output=True, text=True)
    if p.returncode != 0:
        log("  compile failed:\n" + p.stderr[-2500:])
        return None
    return exe


def run_one(row: Row, verbose: bool) -> Result:
    lines = []
    log = lines.append
    res = Result(row=row)
    log(f"=== {row.problem}  [{row.templates}]")

    if row.driver == "-":
        res.verdict, res.detail = "BLOCKED", row.notes
        log(f"  BLOCKED: {row.notes}")
        return finish(res, lines, verbose)

    if not generate(row.dir, log):
        res.verdict, res.detail = "GENFAIL", "test data generation failed"
        return finish(res, lines, verbose)

    pdir = LCP / row.dir
    with open(pdir / "info.toml", "rb") as f:
        res.timelimit = float(tomllib.load(f).get("timelimit", 10.0))

    exe = compile_driver(row, log)
    if exe is None:
        res.verdict, res.detail = "CE", "driver did not compile"
        return finish(res, lines, verbose)

    checker = pdir / "checker"
    ins = sorted((pdir / "in").glob("*.in"))
    if not ins:
        res.verdict, res.detail = "GENFAIL", "no test cases"
        return finish(res, lines, verbose)

    out_dir = BUILD / row.problem.replace("/", "_").replace("#", "_")
    out_dir.mkdir(parents=True, exist_ok=True)
    worst = "AC"
    hard_limit = max(res.timelimit * 3, res.timelimit + 5)

    for inf in ins:
        ansf = out_dir / (inf.stem + ".ans")
        t0 = time.monotonic()
        try:
            with open(inf) as fi, open(ansf, "w") as fo:
                p = subprocess.run([str(exe)], stdin=fi, stdout=fo,
                                   stderr=subprocess.DEVNULL, timeout=hard_limit)
            el = time.monotonic() - t0
        except subprocess.TimeoutExpired:
            el = hard_limit
            res.failures.append((inf.stem, "TLE", f"{el:.2f}s"))
            worst = "TLE"
            if el > res.max_time:
                res.max_time, res.max_case = el, inf.stem
            continue

        if el > res.max_time:
            res.max_time, res.max_case = el, inf.stem
        res.cases += 1

        if p.returncode != 0:
            res.failures.append((inf.stem, "RE", f"exit {p.returncode}"))
            worst = "RE" if worst == "AC" else worst
            continue
        if el > res.timelimit:
            res.failures.append((inf.stem, "TLE", f"{el:.2f}s > {res.timelimit}s"))
            worst = "TLE"
            continue
        # testlib order: <input> <participant output> <jury answer>.
        c = subprocess.run([str(checker), str(inf), str(ansf),
                            str(pdir / "out" / (inf.stem + ".out"))],
                           capture_output=True, text=True)
        if c.returncode != 0:
            msg = (c.stderr or c.stdout).strip().splitlines()
            res.failures.append((inf.stem, "WA", msg[0][:120] if msg else ""))
            worst = "WA"

    res.verdict = worst
    log(f"  {worst}  {res.cases} cases  max {res.max_time:.3f}s / {res.timelimit}s "
        f"({res.ratio*100:.0f}%) on {res.max_case}")
    for name, kind, msg in res.failures[:5]:
        log(f"    {kind} {name}: {msg}")
    if len(res.failures) > 5:
        log(f"    ... and {len(res.failures) - 5} more")
    return finish(res, lines, verbose)


def finish(res, lines, verbose):
    if verbose or res.verdict != "AC":
        print("\n".join(lines), flush=True)
    else:
        print(lines[-1] if lines else "", flush=True)
    return res


def write_report(results: list[Result], path: Path, jobs: int):
    ok = sum(r.verdict == "AC" for r in results)
    slow = [r for r in results if r.verdict == "AC" and r.ratio > SLOW_RATIO]
    out = [
        "# Library Checker verification results",
        "",
        f"-j {jobs}  ",
        f"{ok} / {len(results)} AC" + (f"  ·  {len(slow)} flagged slow (>{int(SLOW_RATIO*100)}% of TL)" if slow else ""),
        "",
    ]
    if jobs > 1:
        out += [
            "> Verdicts do not depend on `-j`, but **these times do**: against `-j 1`",
            f"> the median row inflates ~1.8x and the worst ~4x, since the {jobs} workers",
            "> share memory bandwidth. Re-run with `-j 1` before trusting a % of TL,",
            "> or before concluding that a change made something slower.",
            "",
        ]
    out += [
        "| codebook template | LC problem | verdict | cases | max time | TL | % of TL |",
        "|---|---|---|---|---:|---:|---:|",
    ]
    for r in sorted(results, key=lambda r: (r.verdict == "AC", r.row.problem)):
        mark = " ⚠" if r.verdict == "AC" and r.ratio > SLOW_RATIO else ""
        out.append(
            f"| `{r.row.templates}` | `{r.row.problem}` | **{r.verdict}**{mark} | {r.cases} | "
            f"{r.max_time:.3f}s | {r.timelimit:g}s | {r.ratio*100:.0f}% |"
        )
    bad = [r for r in results if r.verdict not in ("AC", "BLOCKED")]
    blocked = [r for r in results if r.verdict == "BLOCKED"]
    if blocked:
        out += ["", "## Blocked (template cannot be driven as written)", ""]
        for r in blocked:
            out.append(f"- `{r.row.templates}` → `{r.row.problem}`: {r.row.notes}")
    if bad:
        out += ["", "## Failures", ""]
        for r in bad:
            out.append(f"### `{r.row.problem}` — {r.verdict}")
            if r.detail:
                out.append(f"- {r.detail}")
            for name, kind, msg in r.failures[:10]:
                out.append(f"- {kind} `{name}`: {msg}")
            out.append("")
    path.write_text("\n".join(out) + "\n")
    print(f"\nwrote {path}  ({ok}/{len(results)} AC)")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-p", "--problem", action="append", default=[],
                    help="substring filter on the LC problem path (repeatable)")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("-j", "--jobs", type=int, default=max(1, (os.cpu_count() or 4) - 1))
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("-o", "--out", default=None)
    ap.add_argument("--map", default=str(HERE / "MAP.tsv"))
    args = ap.parse_args()

    # deep-recursion templates (Tarjan &co) need a big stack; children inherit this
    try:
        resource.setrlimit(resource.RLIMIT_STACK,
                           (resource.RLIM_INFINITY, resource.RLIM_INFINITY))
    except (ValueError, OSError):
        pass

    if not PY.exists():
        sys.exit(f"no venv python at {PY}; run `uv venv && uv pip install colorlog` in {HERE}")

    rows = read_map(Path(args.map))
    if args.problem:
        rows = [r for r in rows if any(q in r.problem or q in r.templates for q in args.problem)]
    elif not args.all:
        sys.exit("pass --all or -p <filter>")
    if not rows:
        sys.exit("nothing matched")

    print(f"{len(rows)} problem(s), {args.jobs} jobs\n")
    t0 = time.monotonic()
    with cf.ThreadPoolExecutor(max_workers=args.jobs) as ex:
        results = list(ex.map(lambda r: run_one(r, args.verbose), rows))
    print(f"\ntotal {time.monotonic() - t0:.1f}s")

    out = Path(args.out) if args.out else HERE / "RESULT.md"
    write_report(results, out, args.jobs)
    return 0 if all(r.verdict in ("AC", "BLOCKED") for r in results) else 1


if __name__ == "__main__":
    sys.exit(main())
