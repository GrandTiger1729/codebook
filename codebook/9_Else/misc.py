from decimal import *
setcontext(Context(prec=MAX_PREC, Emax=MAX_EMAX,
                   rounding=ROUND_HALF_UP))
print(Decimal(input()) * Decimal(input()))
from fractions import Fraction
Fraction('3.14159').limit_denominator(10).numerator # 22
from math import isqrt
def pell(D): # min x, y > 0: x*x - D*y*y == 1
  a0 = a = isqrt(D) # D must not be a square
  m, d, p, q, pp, qq = 0, 1, a0, 1, 1, 0
  while p * p - D * q * q != 1:
    m = d * a - m; d = (D - m * m) // d
    a = (a0 + m) // d
    p, q, pp, qq = a * p + pp, a * q + qq, p, q
  return p, q
