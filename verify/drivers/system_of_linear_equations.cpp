#include "../prelude.h"
const int MOD = 998244353, MAXN = 505;
int pw(int a, ll b) {
  ll r = 1, x = a;
  for (; b; b >>= 1, x = x * x % MOD) if (b & 1) r = r * x % MOD;
  return (int)r;
}
struct mint {
  int v = 0;
  mint(ll x = 0) : v((int)(((x % MOD) + MOD) % MOD)) {}
  mint operator-() const { return mint(-v); }
  mint operator+(mint o) const { return mint(v + o.v); }
  mint operator*(mint o) const { return mint((ll)v * o.v); }
  mint operator/(mint o) const { return mint((ll)v * pw(o.v, MOD - 2)); }
  bool operator==(mint o) const { return v == o.v; }
};
#include "../../codebook/6_Math/Linear_Equations.cpp"

matrix<mint> mat;

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  mat.n = n, mat.m = m;
  FOR (i, 0, n - 1) FOR (j, 0, m - 1) {
    ll x; cin >> x;
    mat.M[i][j] = mint(x);
  }
  FOR (i, 0, n - 1) {
    ll x; cin >> x;
    mat.M[i][m] = mint(x);
  }
  int r = mat.solve();
  if (r < 0) { cout << "-1\n"; return 0; }
  cout << r << '\n';
  FOR (j, 0, m - 1) cout << mat.sol[j].v << " \n"[j + 1 == m];
  FOR (i, 0, r - 1)
    FOR (j, 0, m - 1) cout << mat.basis[i][j].v << " \n"[j + 1 == m];
}
