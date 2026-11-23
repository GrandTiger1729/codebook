import sys, math
input = sys.stdin.readline  # keeps the '\n'
sys.setrecursionlimit(1 << 20)
sys.set_int_max_str_digits(0)  # 3.11+: ints > 4300 digits
pow(3, -1, 7), math.isqrt(10**20), math.comb(50, 25)  # 3.8+
from decimal import *
# MAX_PREC: exact + - * only, / and sqrt raise MemoryError
setcontext(Context(prec=MAX_PREC, Emax=MAX_EMAX,
                   rounding=ROUND_HALF_UP))
print(Decimal(input()) * Decimal(input()))
getcontext().prec = 50; print(Decimal(2).sqrt())
from fractions import Fraction
Fraction('3.14159').limit_denominator(10).numerator # 22
