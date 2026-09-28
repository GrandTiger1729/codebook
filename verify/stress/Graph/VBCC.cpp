#include "prelude.h"
#include "stress.h"
const int MAXN = 20;
vector<int> adj[MAXN];
#include "2_Graph/VBCC.cpp"
// Reference: kactl BiconnectedComponents.h (CC0), an independent edge-based
// implementation. Its edge lists become vertex sets; a vertex with no edges
// is a block of its own. The two sets of blocks must be equal.
namespace kactl {
typedef vector<int> vi;
vi num, st;
vector<vector<pair<int, int>>> ed;
int Time;
template <class F> int dfs(int at, int par, F &f) {
  int me = num[at] = ++Time, top = me;
  for (auto [y, e] : ed[at]) if (e != par) {
    if (num[y]) {
      top = min(top, num[y]);
      if (num[y] < me) st.pb(e);
    } else {
      int si = st.size(), up = dfs(y, e, f);
      top = min(top, up);
      if (up == me) {
        st.pb(e), f(vi(st.begin() + si, st.end()));
        st.resize(si);
      } else if (up < me) st.pb(e);
    }
  }
  return top;
}
template <class F> void bicomps(F f) {
  num.assign(ed.size(), 0), Time = 0;
  FOR (i, 0, (int)ed.size() - 1) if (!num[i]) dfs(i, -1, f);
}
} // namespace kactl
int main() {
  int cases = 0;
  FOR (it, 1, 20000) {
    int n = rnd(1, 10), m = rnd(0, 16);
    vector<pair<int, int>> E;
    FOR (i, 1, n) adj[i].clear();
    FOR (i, 1, m) {
      int a = rnd(1, n), b = rnd(1, 4) == 1 ? a : rnd(1, n); // some self-loops
      E.pb(a, b), adj[a].pb(b), adj[b].pb(a);
      if (rnd(1, 5) == 1) E.pb(a, b), adj[a].pb(b), adj[b].pb(a); // parallel edge
    }
    VBCC g(n); g.work();
    VBCC h(n);                                          // calling tarjan directly on
    FOR (i, 1, n) if (!h.dfn[i]) h.tarjan(i, 0);        // each root must match work()
    assert(h.bccnt == g.bccnt && h.bcc == g.bcc && h.st.empty());
    vector<set<int>> mine(g.bccnt + 1);
    FOR (v, 1, n) {
      assert(!g.bcc[v].empty());
      for (int c : g.bcc[v]) { assert(1 <= c && c <= g.bccnt); mine[c].insert(v); }
    }
    set<set<int>> a(mine.begin() + 1, mine.end()), b;
    assert((int)a.size() == g.bccnt);                   // no empty or duplicate ids
    kactl::ed.assign(n + 1, {});
    FOR (i, 0, (int)E.size() - 1)
      kactl::ed[E[i].F].pb(E[i].S, i), kactl::ed[E[i].S].pb(E[i].F, i);
    vector<int> deg(n + 1);
    for (auto [x, y] : E) if (x != y) deg[x]++, deg[y]++; // self-loops don't count
    kactl::bicomps([&](const vector<int> &es) {
      set<int> s; for (int e : es) s.insert(E[e].F), s.insert(E[e].S);
      b.insert(s);
    });
    FOR (v, 1, n) if (!deg[v]) b.insert({v});           // isolated (or only self-loops)
    for (auto [x, y] : E) if (x != y) {                 // a bridge is a block too
      bool inside = 0;
      for (auto &s : b) inside |= s.count(x) && s.count(y);
      if (!inside) b.insert({x, y});
    }
    assert(a == b);
    cases++;
  }
  printf("VBCC: %d multigraphs (n<=10, parallel edges, self-loops) vs kactl bicomps; direct tarjan == work()\n", cases);
}
