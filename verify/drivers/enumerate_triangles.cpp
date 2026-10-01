#include "../prelude.h"
const int N = 100005;
#include "../../codebook/2_Graph/C3C4.cpp"

int main() {
  Waimai;
  const ll mod = 998244353;
  int n, m; cin >> n >> m;
  vector<ll> x(n);
  for (auto &v : x) cin >> v;
  for (int i = 0; i < m; i++) {
    int u, v; cin >> u >> v;
    G[u].pb(v), G[v].pb(u);
  }
  build(n);
  ll ans = 0;
  C3(n, [&](int a, int b, int c) { ans = (ans + x[a] * x[b] % mod * x[c]) % mod; });
  cout << ans << '\n';
}
