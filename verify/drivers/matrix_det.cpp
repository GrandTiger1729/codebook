#include "../prelude.h"
#include "../../codebook/6_Math/DeterminantAnyMod.cpp"
int main() {
  Waimai;
  int n; ll m; cin >> n; m = 998244353;
  vector<vector<ll>> a(n, vector<ll>(n));
  for (auto &r : a) for (auto &x : r) cin >> x, x %= m;
  cout << det(a, m) << '\n';
}
