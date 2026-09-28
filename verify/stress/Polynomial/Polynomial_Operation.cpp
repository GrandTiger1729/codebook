#include "prelude.h"
#include "stress.h"
#include "6_Math/QuadraticResidue.cpp"
// Side branch (not in the book): our own NTT + free-function Poly.
#include "7_Polynomial/Number_Theory_Transform.cpp"
#include "7_Polynomial/Polynomial_Operation.cpp"
struct mi { // field type for BerlekampMassey
  int v = 0;
  mi(ll x = 0) : v((x % MOD + MOD) % MOD) {}
  mi operator-() const { return mi(sub(0, v)); }
  mi &operator+=(mi o) { v = add(v, o.v); return *this; }
  mi &operator-=(mi o) { v = sub(v, o.v); return *this; }
  mi operator*(mi o) const { return mi(mul(v, o.v)); }
  mi operator/(mi o) const { return mi(mul(v, inv(o.v))); }
  bool operator==(int x) const { return v == x; }
};
#include "6_Math/Berlekamp-Massey.cpp"

int R() { return rnd(0, MOD - 1); }
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
int kitamasa(const Poly &a, const Poly &c1, ll k) { // naive O(d^2 log k), c1 1-based
  int d = SZ(a);
  auto mulmod = [&](const Poly &p, const Poly &q) {
    Poly r(2 * d - 1);
    FOR (i, 0, d - 1) FOR (j, 0, d - 1) r[i + j] = add(r[i + j], mul(p[i], q[j]));
    for (int i = 2 * d - 2; i >= d; i--) FOR (j, 1, d) r[i - j] = add(r[i - j], mul(r[i], c1[j]));
    r.resize(d);
    return r;
  };
  Poly res(d), x(d);
  res[0] = 1;
  if (d == 1) x[0] = c1[1]; else x[1] = 1;
  for (; k; k >>= 1, x = mulmod(x, x)) if (k & 1) res = mulmod(res, x);
  int s = 0;
  FOR (i, 0, d - 1) s = add(s, mul(res[i], a[i]));
  return s;
}

int main() {
  int cnt[6] = {}, terms = 0;
  // LinearRecursion fed straight from BerlekampMassey (both 1-based); d = 1 often
  FOR (it, 1, 600) {
    int d = it % 3 == 0 ? 1 : rnd(1, 20), T = 2 * d + rnd(1, 20);
    Poly c(d + 1), s(T);
    FOR (j, 1, d) c[j] = rnd(0, 3) ? R() : rnd(0, 2);
    FOR (i, 0, T - 1) {
      if (i < d) s[i] = rnd(0, 4) ? R() : 0;
      else FOR (j, 1, d) s[i] = add(s[i], mul(c[j], s[i - j]));
    }
    vector<mi> pre(s.begin(), s.begin() + 2 * d);
    vector<mi> bm = BerlekampMassey(pre);
    int e = SZ(bm) - 1;
    if (e == 0) continue; // all-zero sequence, nothing to recurse on
    Poly a(s.begin(), s.begin() + e), c1(e + 1);
    FOR (j, 1, e) c1[j] = bm[j].v;
    FOR (k, 0, T - 1) assert(LinearRecursion(a, c1, k) == s[k]), terms++;
    ll k = rnd(0, 1) ? rnd(T, 10000) : rnd(0, (ll)1e18);
    assert(LinearRecursion(a, c1, k) == kitamasa(a, c1, k)), terms++;
    cnt[0]++;
  }
  // PowerProj: ret[i] = sum_j w_j [x^j] f^i, i = 0..m, f[0] = 0
  FOR (it, 1, 400) {
    int n = rnd(1, 40), m = rnd(0, 60);
    Poly f = rpoly(n), w = rpoly(n);
    f[0] = 0;
    Poly got = PowerProj(w, f, m), p = {1};
    assert(SZ(got) == m + 1);
    FOR (i, 0, m) {
      int s = 0;
      FOR (j, 0, min(n, SZ(p)) - 1) s = add(s, mul(w[j], p[j]));
      assert(got[i] == s);
      p = naiveMul(p, f), p.resize(n);
    }
    cnt[1]++;
  }
  // Evaluate vs Horner, then Interpolate back at distinct points
  FOR (it, 1, 400) {
    int n = rnd(1, 60), k = rnd(1, 60);
    Poly a = rpoly(n);
    vector<int> x(k);
    for (int &v : x) v = rnd(0, 3) ? R() : rnd(0, 5); // repeats allowed
    vector<int> y = Evaluate(a, x);
    assert(SZ(y) == k);
    FOR (i, 0, k - 1) assert(y[i] == horner(a, x[i]));
    set<int> seen; vector<int> xs, ys;
    while (SZ(xs) < n) { int v = rnd(0, 2) ? R() : rnd(0, 50); if (seen.insert(v).S) xs.pb(v); }
    ys = Evaluate(a, xs);
    assert(samePoly(Interpolate(xs, ys), a));
    cnt[2]++;
  }
  // Divide: a = b q + r, deg r < deg b (r is padded to max(1, m - 1))
  FOR (it, 1, 600) {
    int n = rnd(1, 60), m = rnd(1, 40);
    Poly a = rpoly(n), b = rpoly(m);
    if (!b.back()) b.back() = 1;
    auto [q, r] = Divide(a, b);
    assert(SZ(r) <= max(1, m - 1) || (n < m && r == a));
    if (m == 1) assert(samePoly(r, {}));
    Poly t = naiveMul(b, q);
    t.resize(max(SZ(t), n));
    FOR (i, 0, SZ(r) - 1) t[i] = add(t[i], r[i]);
    assert(samePoly(t, a));
    cnt[3]++;
  }
  // TaylorShift (c may be negative / huge) and SamplingShift vs direct evaluation
  FOR (it, 1, 300) {
    int n = rnd(1, 50);
    ll c = rnd(0, 1) ? rnd(-(ll)4e18, (ll)4e18) : rnd(-5, 5);
    int cm = (c % MOD + MOD) % MOD;
    Poly a = rpoly(n), want(n), pw = {1};
    FOR (i, 0, n - 1) {
      FOR (j, 0, SZ(pw) - 1) want[j] = add(want[j], mul(a[i], pw[j]));
      pw = naiveMul(pw, {cm, 1});
    }
    assert(TaylorShift(a, c) == want);
    int m = rnd(1, 50);
    vector<int> y(n);
    FOR (i, 0, n - 1) y[i] = horner(a, i);
    vector<int> got = SamplingShift(y, c, m);
    assert(SZ(got) == m);
    FOR (i, 0, m - 1) assert(got[i] == horner(a, add(cm, i)));
    cnt[4]++;
  }
  printf("Polynomial_Operation: LinearRecursion %d (%d terms), PowerProj %d, Evaluate/Interpolate %d, Divide %d, Taylor/SamplingShift %d OK\n",
    cnt[0], terms, cnt[1], cnt[2], cnt[3], cnt[4]);
}
