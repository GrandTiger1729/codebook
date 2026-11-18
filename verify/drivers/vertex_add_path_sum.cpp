#include "../prelude.h"
const int N = 500005;
#include "../../codebook/3_Data_Structure/Heavy_light_Decomposition.cpp"

// HLD::query() is deliberately a skeleton -- the segment-tree calls are left as
// comments -- so the driver supplies a BIT over pl[] and walks the path itself.
// This covers the decomposition (dfs/cut/pl/ulink/deep/pa), not the query body.
ll bit[N];
int n;
void upd(int i, ll v) { for (; i <= n; i += i & -i) bit[i] += v; }
ll pre(int i) { ll s = 0; for (; i > 0; i -= i & -i) s += bit[i]; return s; }
ll rng(int l, int r) { return pre(r) - pre(l - 1); }

Heavy_light_Decomposition hld;

int main() {
  Waimai;
  int q; cin >> n >> q;
  vector<ll> a(n + 1);
  hld.init(n);
  for (int i = 1; i <= n; i++) { cin >> a[i]; hld.val[i] = 0; }
  for (int i = 0; i < n - 1; i++) { int u, v; cin >> u >> v; hld.add_edge(u + 1, v + 1); }
  hld.build();
  for (int i = 1; i <= n; i++) upd(hld.pl[i], a[i]);
  while (q--) {
    int t; cin >> t;
    if (t == 0) { int p; ll x; cin >> p >> x; upd(hld.pl[p + 1], x); }
    else {
      int u, v; cin >> u >> v; u++, v++;
      int ta = hld.ulink[u], tb = hld.ulink[v];
      ll res = 0;
      while (ta != tb) {
        if (hld.deep[ta] > hld.deep[tb]) swap(ta, tb), swap(u, v);
        res += rng(hld.pl[tb], hld.pl[v]);
        tb = hld.ulink[v = hld.pa[tb]];
      }
      if (hld.pl[u] > hld.pl[v]) swap(u, v);
      cout << res + rng(hld.pl[u], hld.pl[v]) << '\n';
    }
  }
}
