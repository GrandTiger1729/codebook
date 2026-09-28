#include "prelude.h"
#include "stress.h"
const int MAXN = 8; // the template needs an external MAXN (and a field type T)

struct Frac { // exact rational, always reduced with den > 0
  ll p = 0, q = 1;
  Frac() {}
  Frac(__int128 a, __int128 b = 1) {
    assert(b != 0);
    if (b < 0) a = -a, b = -b;
    __int128 g = a < 0 ? -a : a, h = b;
    while (h) swap(g %= h, h);
    a /= g, b /= g;
    assert(a <= LLONG_MAX && a >= LLONG_MIN && b <= LLONG_MAX);
    p = a, q = b;
  }
  Frac operator+(Frac o) const { return Frac((__int128)p * o.q + (__int128)o.p * q, (__int128)q * o.q); }
  Frac operator-(Frac o) const { return Frac((__int128)p * o.q - (__int128)o.p * q, (__int128)q * o.q); }
  Frac operator*(Frac o) const { return Frac((__int128)p * o.p, (__int128)q * o.q); }
  Frac operator/(Frac o) const { return Frac((__int128)p * o.q, (__int128)q * o.p); }
  Frac operator-() const { return Frac(-(__int128)p, q); }
  bool operator==(Frac o) const { return p == o.p && q == o.q; }
};
struct Mint { // Z / 998244353
  static const int P = 998244353;
  ll v = 0;
  Mint() {}
  Mint(ll x) : v((x % P + P) % P) {}
  Mint operator+(Mint o) const { return Mint(v + o.v); }
  Mint operator-(Mint o) const { return Mint(v - o.v); }
  Mint operator*(Mint o) const { return Mint(v * o.v); }
  Mint operator-() const { return Mint(-v); }
  Mint operator/(Mint o) const {
    assert(o.v);
    ll r = 1, b = o.v;
    for (ll e = P - 2; e; e >>= 1, b = b * b % P) if (e & 1) r = r * b % P;
    return *this * Mint(r);
  }
  bool operator==(Mint o) const { return v == o.v; }
};
#include "6_Math/Linear_Equations.cpp"

template <typename T> int rankOf(vector<vector<T>> a) { // independent row reduction
  int r = 0, n = a.size(), m = n ? a[0].size() : 0;
  for (int c = 0; c < m && r < n; c++) {
    int p = r;
    while (p < n && a[p][c] == T()) p++;
    if (p == n) continue;
    swap(a[p], a[r]);
    for (int i = r + 1; i < n; i++) {
      T t = a[i][c] / a[r][c];
      for (int j = c; j < m; j++) a[i][j] = a[i][j] - t * a[r][j];
    }
    r++;
  }
  return r;
}
template <typename T> struct Stats { int cases = 0, bad = 0, free = 0; };

// A: n x m, rows are random combos of `rk` random rows so rank <= rk
template <typename T> void run(Stats<T> &st, int lo, int hi, bool plant) {
  int n = rnd(1, MAXN), m = rnd(1, MAXN), rk = rnd(0, min(n, m));
  vector<vector<T>> base(rk, vector<T>(m)), A(n, vector<T>(m));
  for (auto &r : base) for (auto &x : r) x = T(rnd(lo, hi));
  for (auto &r : A) {
    for (auto &b : base) { T c = T(rnd(-1, 1)); FOR (j, 0, m - 1) r[j] = r[j] + c * b[j]; }
    if (rnd(0, 9) == 0) FOR (j, 0, m - 1) r[j] = T(rnd(lo, hi)); // occasional full-random row
  }
  vector<T> b(n);
  if (plant) { // consistent: b = A x0
    vector<T> x0(m);
    for (auto &x : x0) x = T(rnd(lo, hi));
    FOR (i, 0, n - 1) FOR (j, 0, m - 1) b[i] = b[i] + A[i][j] * x0[j];
  } else for (auto &x : b) x = T(rnd(lo, hi));
  auto Ab = A;
  FOR (i, 0, n - 1) Ab[i].pb(b[i]);
  int rA = rankOf(A), rAb = rankOf(Ab);

  auto *mt = new matrix<T>(); // value-initialised: fixed[], sol[], basis[] start at 0
  mt->n = n, mt->m = m;
  FOR (i, 0, n - 1) FOR (j, 0, m) mt->M[i][j] = Ab[i][j];
  int d = mt->solve();
  if (rA != rAb) { assert(d == -1); st.bad++; }
  else {
    assert(d == m - rA);
    FOR (i, 0, n - 1) { // A sol = b
      T s;
      FOR (j, 0, m - 1) s = s + A[i][j] * mt->sol[j];
      assert(s == b[i]);
    }
    vector<vector<T>> B(d, vector<T>(m));
    FOR (k, 0, d - 1) {
      FOR (j, 0, m - 1) B[k][j] = mt->basis[k][j];
      FOR (i, 0, n - 1) { // A basis_k = 0
        T s;
        FOR (j, 0, m - 1) s = s + A[i][j] * B[k][j];
        assert(s == T());
      }
    }
    if (d) assert(rankOf(B) == d); // basis vectors independent, so they span the kernel
    st.free += d;
  }
  st.cases++;
  delete mt;
}
int main() {
  Stats<Frac> fr; Stats<Mint> md;
  FOR (it, 1, 3000) run(fr, -3, 3, rnd(0, 1));
  FOR (it, 1, 3000) run(md, 0, Mint::P - 1, rnd(0, 1));
  assert(fr.bad && md.bad && fr.free && md.free);
  printf("Linear_Equations: %d fraction + %d modint systems OK (%d + %d inconsistent, %d + %d kernel vectors)\n",
         fr.cases, md.cases, fr.bad, md.bad, fr.free, md.free);
}
