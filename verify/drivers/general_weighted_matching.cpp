#include "../prelude.h"
const int INF = INT_MAX;  // Maximum_Weight_Matching uses INF; the codebook never defines it
#include "../../codebook/4_Flow_Matching/Maximum_Weight_Matching.cpp"

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  WeightGraph g(n);  // 1-based
  while (m--) {
    int u, v, w; cin >> u >> v >> w;
    g.add_edge(u + 1, v + 1, w);
  }
  auto [tot, k] = g.solve();
  cout << k << ' ' << tot << '\n';
  for (int u = 1; u <= n; u++)
    if (g.match[u] && g.match[u] > u) cout << u - 1 << ' ' << g.match[u] - 1 << '\n';
}
