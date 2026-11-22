#!/bin/sh
# Build the codebook the same way CI does.
# LaTeX build files go to build/, the finished PDF to pdf/codebook.pdf.
# Usage: ./build.sh          build
#        ./build.sh clean    remove build/
set -e
root=$(cd "$(dirname "$0")" && pwd)
if [ "$1" = clean ]; then rm -rf "$root/build"; exit; fi
python3 "$root/verify/check_layout.py"
mkdir -p "$root/build" "$root/pdf"
cd "$root/codebook"
if ! latexmk -xelatex -shell-escape -interaction=nonstopmode -halt-on-error \
     -auxdir="$root/build" -outdir="$root/pdf" codebook.tex > /dev/null 2>&1; then
  grep -A5 '^!' "$root/build/codebook.log"
  exit 1
fi
pages=$(pdfinfo "$root/pdf/codebook.pdf" | awk '/^Pages:/ {print $2}')
echo "pdf/codebook.pdf: $pages pages (limit 25)"
[ "$pages" -le 25 ]
