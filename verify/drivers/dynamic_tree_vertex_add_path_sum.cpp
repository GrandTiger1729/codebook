#include "../prelude.h"
const int N = 200005;
#include "../../codebook/3_Data_Structure/link_cut_tree.cpp"

LCT t;
ll a[N];

int main() {
  Waimai;
  int n, q; cin >> n >> q;
  t.init(n);
  FOR (i, 1, n) cin >> a[i], t.change(i, a[i]);
  FOR (i, 1, n - 1) {
    int u, v; cin >> u >> v;
    t.link(u + 1, v + 1);
  }
  while (q--) {
    int op; cin >> op;
    if (op == 0) {
      int u, v, w, x; cin >> u >> v >> w >> x;
      t.cut(u + 1, v + 1), t.link(w + 1, x + 1);
    } else if (op == 1) {
      int p; ll x; cin >> p >> x;
      p++, a[p] += x, t.change(p, a[p]);
    } else {
      int u, v; cin >> u >> v;
      cout << t.query(u + 1, v + 1) << '\n';
    }
  }
}
