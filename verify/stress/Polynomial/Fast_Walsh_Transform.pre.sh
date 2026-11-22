# The book prints all three butterfly lines (// OR, // AND, // XOR); a contestant keeps one.
# Write one copy per choice: fwt_OR.h keeps subset_convolution, the others stop after fwt.
src=$1/7_Polynomial/Fast_Walsh_Transform.cpp
for op in OR AND XOR; do
  others=$(echo "OR AND XOR" | tr ' ' '\n' | grep -vx $op | paste -sd'|')
  grep -v -E "// ($others)\$" "$src" > "$2/fwt_$op.h"
  [ $op = OR ] || sed -i '/^} \/\/ XOR inv/q' "$2/fwt_$op.h"
  [ $(grep -c -E '// (OR|AND|XOR)$' "$2/fwt_$op.h") -ge 1 ] || exit 1
done
