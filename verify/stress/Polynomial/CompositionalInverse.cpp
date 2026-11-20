#include "prelude.h"
#include "stress.h"
#include "6_Math/QuadraticResidue.cpp"
#include "mod_helpers.h"
#include "7_Polynomial/NTT_stdabs.cpp"
#include "7_Polynomial/Operation_stdabs.cpp"
#include "7_Polynomial/CompositionalInverse.cpp"

// f(g(x)) == x mod x^n by naive composition (Horner on truncated polys)
Poly naiveMul(const Poly &a, const Poly &b, int n) {
  Poly c(n);
  FOR (i, 0, SZ(a) - 1) FOR (j, 0, min(SZ(b), n - i) - 1)
    c[i + j] = add(c[i + j], mul(a[i], b[j]));
  return c;
}
int main() {
  int cnt = 0;
  FOR (t, 1, 20000) {
    int n = rnd(1, t <= 10000 ? 8 : 40);
    Poly f(n);
    FOR (i, 1, n - 1) f[i] = rnd(0, 1) ? rnd(0, 3) : rnd(0, MOD - 1);
    if (n > 1 && !f[1]) f[1] = rnd(1, MOD - 1);
    Poly g = CompInv(f), c(n);
    assert(SZ(g) == n && g[0] == 0);
    for (int i = n - 1; i >= 0; i--) c = naiveMul(c, g, n), c[0] = add(c[0], f[i]);
    Poly x(n);
    if (n > 1) x[1] = 1;
    assert(c == x), cnt++;
  }
  printf("CompInv: %d cases OK (n = 1..40)\n", cnt);
}
