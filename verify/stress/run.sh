#!/usr/bin/env bash
# Local stress tests (kactl style: plain main + assert, exit 0 = pass).
# Tests #include templates as "8_Geometry/Heart.cpp", resolved in this repo.
#   ./run.sh                    # every stress/*/*.cpp and */*.py
#   ./run.sh Geometry/Heart.cpp # just one
#   SAN=0 ./run.sh              # -O2 without sanitizers
set -u
HERE=$(cd "$(dirname "$0")" && pwd)
CB=$HERE/../../codebook
SAN=${SAN:-1}
OUT=$HERE/build; mkdir -p "$OUT"
FLAGS="-std=c++20 -O2 -I$CB -I$HERE/.. -I$HERE -w"
[ "$SAN" = 1 ] && FLAGS="$FLAGS -g -fsanitize=address,undefined -fno-sanitize-recover=all -D_GLIBCXX_DEBUG"
cd "$HERE"
files=("$@"); [ ${#files[@]} = 0 ] && files=(*/*.cpp */*.py)
pass=0; fail=0
# In GitHub Actions, turn each failure into an annotation on the PR.
annotate() { [ -n "${GITHUB_ACTIONS:-}" ] || return 0
  printf '::error file=verify/stress/%s,title=%s::%s\n' "$1" "$2" \
    "$(head -c 2000 "$3" | sed ':a;N;$!ba;s/%/%25/g;s/\r/%0D/g;s/\n/%0A/g')"; }
for f in "${files[@]}"; do
  exe=$OUT/$(echo "${f%.*}" | tr / _)
  if [[ $f == *.py ]]; then # Python snippets (misc.py): run as is
    printf '#!/bin/sh\nexec python3 "%s"\n' "$HERE/$f" > "$exe"; chmod +x "$exe"
  elif ! g++ $FLAGS "$f" -o "$exe" 2>"$exe.cerr"; then
    echo "COMPILE FAIL  $f"; head -5 "$exe.cerr"; annotate "$f" "compile failed" "$exe.cerr"
    fail=$((fail+1)); continue
  fi
  start=$EPOCHREALTIME
  if timeout 300 "$exe" >"$exe.out" 2>&1; then
    printf "PASS  %-50s %6.1fs  %s\n" "$f" "$(awk "BEGIN{print $EPOCHREALTIME - $start}")" "$(tail -1 "$exe.out")"
    pass=$((pass+1))
  else
    echo "FAIL  $f"; tail -5 "$exe.out"; tail -20 "$exe.out" > "$exe.tail"
    annotate "$f" "stress test failed" "$exe.tail"; fail=$((fail+1))
  fi
done
echo "$pass passed, $fail failed"
[ $fail = 0 ]
