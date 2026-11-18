// one driver for both LC problems: -DDIRECTED for eulerian_trail_directed
#include "../prelude.h"
#include "../../codebook/2_Graph/EulerTrail.cpp"
int main() {
  Waimai;
  int t; cin >> t;
  while (t--) {
    int n, m; cin >> n >> m;
    vector<vector<pair<int, int>>> g(n);
    vector<int> bal(n, 0), deg(n, 0);
    FOR (i, 0, m - 1) {
      int u, v; cin >> u >> v;
      g[u].pb(v, i), deg[u]++, deg[v]++;
#ifdef DIRECTED
      bal[u]++, bal[v]--;
#else
      if (u != v) g[v].pb(u, i);
      bal[u] ^= 1, bal[v] ^= 1;
#endif
    }
    int s = -1, bad = 0;
    FOR (v, 0, n - 1) {
#ifdef DIRECTED
      if (bal[v] == 1) { if (s >= 0 && bal[s] == 1) bad = 1; s = v; }
      else if (bal[v] != 0 && bal[v] != -1) bad = 1;
#else
      if (bal[v]) s = (s >= 0 && bal[s]) ? s : v;
#endif
    }
#ifndef DIRECTED
    int odd = 0; FOR (v, 0, n - 1) odd += bal[v]; if (odd > 2) bad = 1;
#endif
    if (s < 0) { s = 0; FOR (v, 0, n - 1) if (deg[v]) { s = v; break; } }
    auto [vs, es] = euler_trail(s, g, m);
    if (bad || (m > 0 && vs.empty())) { cout << "No\n"; continue; }
    if (m == 0) vs = {s};
    cout << "Yes\n";
    FOR (i, 0, (int)vs.size() - 1) cout << vs[i] << " \n"[i + 1 == (int)vs.size()];
    FOR (i, 0, (int)es.size() - 1) cout << es[i] << ' ';
    cout << '\n';
  }
}
