#!/usr/bin/env bash
# Local stress tests (kactl style: plain main + assert, exit 0 = pass).
# Tests #include templates as "8_Geometry/Heart.cpp", resolved in this repo.
#   ./run.sh                    # every stress/*/*.cpp
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
files=("$@"); [ ${#files[@]} = 0 ] && files=(*/*.cpp)
pass=0; fail=0
for f in "${files[@]}"; do
  exe=$OUT/$(echo "${f%.cpp}" | tr / _)
  if ! g++ $FLAGS "$f" -o "$exe" 2>"$exe.cerr"; then
    echo "COMPILE FAIL  $f"; head -5 "$exe.cerr"; fail=$((fail+1)); continue
  fi
  start=$EPOCHREALTIME
  if timeout 300 "$exe" >"$exe.out" 2>&1; then
    printf "PASS  %-50s %6.1fs  %s\n" "$f" "$(awk "BEGIN{print $EPOCHREALTIME - $start}")" "$(tail -1 "$exe.out")"
    pass=$((pass+1))
  else
    echo "FAIL  $f"; tail -5 "$exe.out"; fail=$((fail+1))
  fi
done
echo "$pass passed, $fail failed"
[ $fail = 0 ]
