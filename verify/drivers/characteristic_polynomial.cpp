#include "../prelude.h"
#include "../../codebook/6_Math/CharPoly.cpp"
int main() {
  Waimai;
  int n; cin >> n;
  vector<vector<ll>> a(n, vector<ll>(n));
  for (auto &r : a) for (auto &x : r) cin >> x;
  auto f = char_poly(a, 998244353);
  FOR (i, 0, n) cout << f[i] << " \n"[i == n];
}
