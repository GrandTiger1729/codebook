# Checks pell() from 9_Else/misc.py: the snippet is exec'd from the file, so
# this tests exactly what is printed in the codebook.
import pathlib
from math import isqrt
src = pathlib.Path(__file__).resolve().parents[3] / "codebook/9_Else/misc.py"
code = src.read_text()
exec(code[code.index("from math import isqrt"):])
n = 0
for D in range(2, 2000):
    if isqrt(D) ** 2 == D:
        continue
    x, y = pell(D)
    assert x > 0 and y > 0 and x * x - D * y * y == 1
    if y <= 200000:  # minimality: no smaller y works
        for yy in range(1, y):
            t = 1 + D * yy * yy
            assert isqrt(t) ** 2 != t
    n += 1
assert pell(61) == (1766319049, 226153980)
print(f"pell ok: {n} values of D < 2000")
