#include "prelude.h"
#include "stress.h"
#include "6_Math/QuadraticResidue.cpp"
#include "mod_helpers.h"
#include "7_Polynomial/NTT_stdabs.cpp"
#include "7_Polynomial/Operation_stdabs.cpp" // supplies Mul, Divide
#include "7_Polynomial/FastLinearRecursion.cpp"
struct mi { // field type for BerlekampMassey
  int v = 0;
  mi(ll x = 0) : v((x % mod + mod) % mod) {}
  mi operator-() const { return mi(sub(0, v)); }
  mi &operator+=(mi o) { v = add(v, o.v); return *this; }
  mi &operator-=(mi o) { v = sub(v, o.v); return *this; }
  mi operator*(mi o) const { return mi(mul(v, o.v)); }
  mi operator/(mi o) const { return mi(mul(v, Pow(o.v, mod - 2))); }
  bool operator==(int x) const { return v == x; }
};
#include "6_Math/Berlekamp-Massey.cpp"

// Random order-d recurrence -> 2d-term prefix -> BerlekampMassey (1-based c) ->
// shift to BostanMori's 0-based c -> must replay every term of a longer
// run, and agree with a naive O(d^2 log k) Kitamasa for huge k.
int R() { return rnd(0, mod - 1); }
int kitamasa(const vector<int> &a, const vector<int> &c1, ll k) { // c1 1-based
  int d = SZ(a);
  auto mulmod = [&](const vector<int> &p, const vector<int> &q) {
    vector<int> r(2 * d - 1);
    FOR (i, 0, d - 1) FOR (j, 0, d - 1) r[i + j] = add(r[i + j], mul(p[i], q[j]));
    for (int i = 2 * d - 2; i >= d; i--) FOR (j, 1, d) r[i - j] = add(r[i - j], mul(r[i], c1[j]));
    r.resize(d);
    return r;
  };
  vector<int> res(d), x(d);
  res[0] = 1;
  if (d == 1) x[0] = c1[1]; else x[1] = 1;
  for (; k; k >>= 1, x = mulmod(x, x)) if (k & 1) res = mulmod(res, x);
  int s = 0;
  FOR (i, 0, d - 1) s = add(s, mul(res[i], a[i]));
  return s;
}
int main() {
  int cases = 0, terms = 0;
  FOR (it, 1, 300) {
    int d = it <= 20 ? 1 : rnd(1, 20), T = 2 * d + rnd(1, 20);
    vector<int> c(d + 1); // 1-based generator
    FOR (j, 1, d) c[j] = rnd(0, 3) ? R() : rnd(0, 2);
    vector<int> s(T);
    FOR (i, 0, T - 1) {
      if (i < d) s[i] = rnd(0, 4) ? R() : 0;
      else FOR (j, 1, d) s[i] = add(s[i], mul(c[j], s[i - j]));
    }
    vector<mi> pre(2 * d);
    FOR (i, 0, 2 * d - 1) pre[i] = s[i];
    vector<mi> bm = BerlekampMassey(pre);
    int e = SZ(bm) - 1;
    if (e == 0) continue; // all-zero sequence: BostanMori needs |a| >= 1
    vector<int> a(s.begin(), s.begin() + e), c0(e), c1(e + 1);
    FOR (j, 0, e - 1) c0[j] = c1[j + 1] = bm[j + 1].v; // 1-based -> 0-based
    FOR (k, 0, T - 1) assert(BostanMori(a, c0, k) == s[k]), terms++;
    FOR (q, 1, 1) {
      ll k = rnd(0, 1) ? rnd(T, 10000) : rnd(0, (ll)1e18);
      assert(BostanMori(a, c0, k) == kitamasa(a, c1, k)), terms++;
    }
    cases++;
  }
  printf("FastLinearRecursion: %d recurrences (d=1..20), %d terms OK\n", cases, terms);
}
