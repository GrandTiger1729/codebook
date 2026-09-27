#include "../prelude.h"
const int MAXN = 500005;
vector<int> adj[MAXN];   // EBCC reads a global adj, 1-based
#include "../../codebook/2_Graph/EBCC.cpp"

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  while (m--) {
    int a, b; cin >> a >> b;
    adj[a + 1].pb(b + 1), adj[b + 1].pb(a + 1);
  }
  EBCC e(n);
  e.work();
  vector<vector<int>> comp(e.bccnt + 1);
  for (int i = 1; i <= n; i++) comp[e.bcc[i]].pb(i - 1);
  cout << e.bccnt << '\n';
  for (int i = 1; i <= e.bccnt; i++) {
    cout << comp[i].size();
    for (int v : comp[i]) cout << ' ' << v;
    cout << '\n';
  }
}
