#include "prelude.h"
#include "stress.h"
#include "6_Math/QuadraticResidue.cpp"
#include "mod_helpers.h"
#include "7_Polynomial/NTT_stdabs.cpp"
#include "7_Polynomial/Operation_stdabs.cpp"

// Divide / Evaluate / Interpolate / TaylorShift / SamplingShift / power_proj
// against O(n^2)-ish brute force, all mod 998244353.
int R() { return rnd(0, mod - 1); }
Poly rpoly(int n) { Poly a(n); for (int &x : a) x = R(); return a; }
Poly naiveMul(const Poly &a, const Poly &b) {
  Poly c(SZ(a) + SZ(b) - 1);
  FOR (i, 0, SZ(a) - 1) FOR (j, 0, SZ(b) - 1) c[i + j] = add(c[i + j], mul(a[i], b[j]));
  return c;
}
int horner(const Poly &a, int x) { int r = 0; for (int i = SZ(a) - 1; i >= 0; i--) r = add(mul(r, x), a[i]); return r; }
bool samePoly(Poly a, Poly b) { // equal up to trailing zeros
  while (SZ(a) && !a.back()) a.pop_back();
  while (SZ(b) && !b.back()) b.pop_back();
  return a == b;
}
vector<int> distinctPts(int n) {
  set<int> s; vector<int> x;
  while (SZ(x) < n) { int v = rnd(0, 2) ? R() : rnd(0, 50); if (s.insert(v).S) x.pb(v); }
  return x;
}
int lagrange(const vector<int> &y, int c) { // f(0..n-1) = y, return f(c)
  int n = SZ(y), res = 0;
  FOR (i, 0, n - 1) {
    int num = 1, den = 1;
    FOR (j, 0, n - 1) if (j != i) num = mul(num, sub(c, j)), den = mul(den, sub(i, j));
    res = add(res, mul(y[i], mul(num, Pow(den, mod - 2))));
  }
  return res;
}

int main() {
  build();
  int cnt[6] = {};
  // Divide: a = b q + r, deg r < deg b
  FOR (it, 1, 500) {
    int n = rnd(1, 60), m = rnd(1, 40);
    Poly a = rpoly(n), b = rpoly(m);
    if (!b.back()) b.back() = 1;
    auto [q, r] = Divide(a, b);
    assert(SZ(r) <= m - 1 || (n < m && r == a));
    Poly t = naiveMul(b, q);
    t.resize(max(SZ(t), n));
    FOR (i, 0, SZ(r) - 1) t[i] = add(t[i], r[i]);
    assert(samePoly(t, a));
    cnt[0]++;
  }
  // Evaluate vs Horner
  FOR (it, 1, 300) {
    int n = rnd(1, 60), k = rnd(1, 60);
    Poly a = rpoly(n);
    vector<int> x(k);
    for (int &v : x) v = rnd(0, 3) ? R() : rnd(0, 5); // repeats allowed
    vector<int> y = Evaluate(a, x);
    assert(SZ(y) == k);
    FOR (i, 0, k - 1) assert(y[i] == horner(a, x[i]));
    cnt[1]++;
  }
  // Interpolate: deg < n poly -> values at n distinct points -> same poly back
  FOR (it, 1, 300) {
    int n = rnd(1, 60);
    Poly a = rpoly(n);
    vector<int> x = distinctPts(n), y(n);
    FOR (i, 0, n - 1) y[i] = horner(a, x[i]);
    assert(samePoly(Interpolate(x, y), a));
    cnt[2]++;
  }
  // TaylorShift vs sum a_i (x + c)^i expanded directly
  FOR (it, 1, 300) {
    int n = rnd(1, 60), c = rnd(0, 1) ? R() : rnd(0, 3);
    Poly a = rpoly(n), want(n), pw = {1};
    FOR (i, 0, n - 1) {
      FOR (j, 0, SZ(pw) - 1) want[j] = add(want[j], mul(a[i], pw[j]));
      pw = naiveMul(pw, {c, 1});
    }
    assert(TaylorShift(a, c) == want);
    cnt[3]++;
  }
  // SamplingShift vs Lagrange
  FOR (it, 1, 200) {
    int n = rnd(1, 40), m = rnd(1, 40), c = rnd(0, 2) ? R() : rnd(0, 60);
    Poly f = rpoly(n);
    vector<int> y(n);
    FOR (i, 0, n - 1) y[i] = horner(f, i);
    vector<int> got = SamplingShift(y, c, m);
    assert(SZ(got) == m);
    FOR (i, 0, m - 1) assert(got[i] == horner(f, add(c, i)) && got[i] == lagrange(y, add(c, i)));
    cnt[4]++;
  }
  // power_proj: ret[i] = sum_j w_j [x^j] f^i, i = 0..m, |w| = |f|, f[0] = 0
  FOR (it, 1, 250) {
    int n = rnd(1, 40), m = rnd(0, 60);
    Poly f = rpoly(n), w = rpoly(n);
    f[0] = 0;
    Poly got = power_proj(w, f, m), p = {1};
    assert(SZ(got) == m + 1);
    FOR (i, 0, m) {
      int s = 0;
      FOR (j, 0, min(n, SZ(p)) - 1) s = add(s, mul(w[j], p[j]));
      assert(got[i] == s);
      p = naiveMul(p, f), p.resize(n);
    }
    cnt[5]++;
  }
  printf("Operation_stdabs: Divide %d, Evaluate %d, Interpolate %d, TaylorShift %d, SamplingShift %d, power_proj %d OK\n",
    cnt[0], cnt[1], cnt[2], cnt[3], cnt[4], cnt[5]);
}
