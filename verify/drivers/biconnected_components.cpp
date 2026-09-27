#include "../prelude.h"
const int MAXN = 500005;
vector<int> adj[MAXN];   // VBCC reads a global adj, 1-based
#include "../../codebook/2_Graph/VBCC.cpp"

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  while (m--) {
    int a, b; cin >> a >> b;
    adj[a + 1].pb(b + 1), adj[b + 1].pb(a + 1);
  }
  VBCC v(n);
  v.work();
  // VBCC gives, per vertex, the list of components it belongs to; invert it
  vector<vector<int>> comp(v.bccnt + 1);
  for (int i = 1; i <= n; i++) for (int c : v.bcc[i]) comp[c].pb(i - 1);
  cout << v.bccnt << '\n';
  for (int i = 1; i <= v.bccnt; i++) {
    cout << comp[i].size();
    for (int x : comp[i]) cout << ' ' << x;
    cout << '\n';
  }
}
