#include "prelude.h"
#include "stress.h"
#include "mod_helpers.h"
#include "7_Polynomial/Lagrange_Interpolation.cpp"

// Random f of degree <= d; y[i] = f(i); Lagrange(y, x) vs Horner at x mod p,
// x small (inside 0..d too), near p, and up to 1e18. Then sum of k-th powers vs a loop.
int horner(const vector<int> &c, ll x) {
  int r = 0, xm = x % MOD;
  for (int i = SZ(c) - 1; i >= 0; i--) r = add(mul(r, xm), c[i]);
  return r;
}
int main() {
  build();
  int cases = 0;
  FOR (t, 1, 100000) {
    int d = t <= 50000 ? rnd(0, 6) : rnd(0, 300);
    vector<int> c(d + 1), y(d + 1);
    for (int &v : c) v = rnd(0, 2) ? rnd(0, MOD - 1) : rnd(0, 2);
    FOR (i, 0, d) y[i] = horner(c, i);
    FOR (q, 0, 3) {
      ll x = q == 0 ? rnd(0, d + 2) : q == 1 ? rnd(MOD - 5, MOD + 5)
           : q == 2 ? rnd(0, (ll)1e18) : rnd(0, 1000);
      assert(Lagrange(y, x) == horner(c, x)), cases++;
    }
    int a = rnd(-1000, 1000); // y[i] = f(a + i), query Lagrange(y, x - a)
    vector<int> z(d + 1);
    FOR (i, 0, d) z[i] = horner(c, ((a + i) % MOD + MOD) % MOD);
    ll x = rnd(-2000, 2000);
    assert(Lagrange(z, x - a) == horner(c, (x % MOD + MOD) % MOD)), cases++;
  }
  int sums = 0;
  FOR (k, 0, 30) {
    vector<int> y(k + 2);
    FOR (i, 1, k + 1) y[i] = add(y[i - 1], Pow(i, k));
    int s = 0;
    FOR (n, 0, 2000) {
      if (n) s = add(s, Pow(n, k));
      assert(Lagrange(y, n) == s), sums++;
    }
  }
  printf("Lagrange_Interpolation: %d evaluations vs Horner, %d power sums OK\n", cases, sums);
}
