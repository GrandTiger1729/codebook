// link_cut_tree vs a brute-force forest: link / cut (also of non-edges, which must be a
// no-op) / change / path-sum query / conn / lca and get_root after chroot.
#include "prelude.h"
#include "stress.h"
const int N = 105;
#include "3_Data_Structure/link_cut_tree.cpp"
LCT t;
int n;
set<pair<int, int>> E;
vector<int> nb(int u) { vector<int> r; for (auto [a, b] : E) { if (a == u) r.pb(b); if (b == u) r.pb(a); } return r; }
vector<int> path(int u, int v) { // vertices on the u-v path, empty if disconnected
  vector<int> par(n + 1, -1); queue<int> q; q.push(u), par[u] = u;
  while (!q.empty()) { int x = q.front(); q.pop(); for (int y : nb(x)) if (par[y] < 0) par[y] = x, q.push(y); }
  if (par[v] < 0) return {};
  vector<int> p{v}; while (p.back() != u) p.pb(par[p.back()]);
  return p;
}
int main() {
  int ops = 0, cuts = 0, fakecuts = 0;
  FOR (it, 1, 3000) {
    n = rnd(1, it % 10 ? 8 : 40);
    t.init(n), E.clear();
    vector<ll> val(n + 1, 0);
    FOR (q, 1, 300) {
      int op = rnd(0, 6), u = rnd(1, n), v = rnd(1, n);
      bool con = !path(u, v).empty();
      if (op == 0 && !con) t.link(u, v), E.insert(minmax(u, v));
      else if (op == 1) {
        bool edge = E.count(minmax(u, v));
        t.cut(u, v), E.erase(minmax(u, v));
        (edge ? cuts : fakecuts)++;
      } else if (op == 2) { ll x = rnd(-1e9, 1e9); t.change(u, x), val[u] = x; }
      else if (op == 3) {
        assert(t.conn(u, v) == con);
        if (con) { ll s = 0; for (int x : path(u, v)) s += val[x]; assert(t.query(u, v) == s); }
      } else if (op == 4 && con) { // lca under root r
        int r = rnd(1, n);
        if (path(r, u).empty()) continue;
        t.chroot(r);
        vector<int> pu = path(r, u), pv = path(r, v);
        set<int> su(pu.begin(), pu.end());
        int l = -1; for (int x : pv) if (su.count(x)) { l = x; break; } // pv runs v -> r
        assert(t.lca(u, v) == l);
        assert(t.get_root(u) == r && t.get_root(v) == r);
      }
      ops++;
    }
  }
  printf("link_cut_tree ok: %d ops (%d cuts of edges, %d cuts of non-edges)\n", ops, cuts, fakecuts);
}
