#include "../prelude.h"
const int N = 200005;
#include "../../codebook/2_Graph/Dominator_Tree.cpp"

dominator_tree dt;

int main() {
  Waimai;
  int n, m, s; cin >> n >> m >> s;
  dt.init(n);
  while (m--) { int a, b; cin >> a >> b; dt.add_edge(a + 1, b + 1); }
  dt.tarjan(s + 1);
  for (int v = 1; v <= n; v++) {
    int d = dt.dfn[v];
    int p = (d == 0) ? -1 : (v == s + 1 ? s : dt.id[dt.idom[d]] - 1);
    cout << p << " \n"[v == n];
  }
}
