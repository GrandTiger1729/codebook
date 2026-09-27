#include "../prelude.h"
const int N = 105, T = 12;
const ll INF = 1e18;
#include "../../codebook/2_Graph/MinimumSteinerTree.cpp"

SteinerTree st;

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  st.init(n);
  FOR (e, 0, m - 1) {
    int u, v; ll w; cin >> u >> v >> w;
    st.add_edge(u, v, w, e), st.add_edge(v, u, w, e);
  }
  int k; cin >> k;
  vector<int> ter(k);
  for (int &x : ter) cin >> x;
  ll y = st.solve(ter);
  auto &es = st.ans;
  cout << y << ' ' << es.size() << '\n';
  FOR (i, 0, (int)es.size() - 1)
    cout << es[i] << " \n"[i + 1 == (int)es.size()];
  if (es.empty()) cout << '\n';
}
