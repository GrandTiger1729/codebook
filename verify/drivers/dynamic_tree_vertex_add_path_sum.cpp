#include "../prelude.h"
#include "../../codebook/3_Data_Structure/link_cut_tree.cpp"

const int N = 200005;
Splay *t[N];
ll a[N];

int main() {
  Waimai;
  int n, q; cin >> n >> q;
  FOR (i, 0, n - 1) {
    cin >> a[i];
    t[i] = new Splay(a[i]);
  }
  FOR (i, 1, n - 1) {
    int u, v; cin >> u >> v;
    link(t[u], t[v]);
  }
  while (q--) {
    int op; cin >> op;
    if (op == 0) {
      int u, v, w, x; cin >> u >> v >> w >> x;
      cut(t[u], t[v]), link(t[w], t[x]);
    } else if (op == 1) {
      int p; ll x; cin >> p >> x;
      a[p] += x, change(t[p], a[p]);
    } else {
      int u, v; cin >> u >> v;
      cout << query(t[u], t[v]) << '\n';
    }
  }
}
