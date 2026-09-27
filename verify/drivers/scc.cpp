#include "../prelude.h"
const int MAXN = 500005;
vector<int> adj[MAXN];   // SCC reads a global adj, 1-based
#include "../../codebook/2_Graph/SCC.cpp"

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  while (m--) { int a, b; cin >> a >> b; adj[a + 1].pb(b + 1); }
  SCC s(n);
  s.work();
  // tarjan numbers components in reverse topological order
  vector<vector<int>> comp(s.sccnt + 1);
  for (int i = 1; i <= n; i++) comp[s.scc[i]].pb(i - 1);
  cout << s.sccnt << '\n';
  for (int c = s.sccnt; c >= 1; c--) {
    cout << comp[c].size();
    for (int v : comp[c]) cout << ' ' << v;
    cout << '\n';
  }
}
