#include "prelude.h"
#include "stress.h"
const int MAXN = 20;
vector<int> adj[MAXN];
#include "2_Graph/EBCC.cpp"
// u, v share a 2-edge-connected component iff they stay connected after
// deleting any single edge. Random multigraphs with self-loops and isolated
// vertices; one EBCC object per case, as a contestant would use it.
int n; vector<pair<int, int>> E;
bool conn(int s, int t, int skip) { // skip: index of the deleted edge
  vector<int> seen(n + 1); vector<int> q{s}; seen[s] = 1;
  while (!q.empty()) {
    int u = q.back(); q.pop_back();
    FOR (i, 0, (int)E.size() - 1) if (i != skip) {
      auto [a, b] = E[i];
      for (int x : {a, b}) if ((x == a ? b : a) == u && !seen[x]) seen[x] = 1, q.pb(x);
    }
  }
  return seen[t];
}
int main() {
  int cases = 0;
  FOR (it, 1, 20000) {
    n = rnd(1, 9); int m = rnd(0, 14); E.clear();
    FOR (i, 1, n) adj[i].clear();
    FOR (i, 1, m) {
      int a = rnd(1, n), b = rnd(1, 4) == 1 ? a : rnd(1, n);  // some self-loops
      E.pb(a, b), adj[a].pb(b), adj[b].pb(a);
      if (rnd(1, 5) == 1) E.pb(a, b), adj[a].pb(b), adj[b].pb(a); // parallel edge
    }
    EBCC e(n); e.work();
    set<int> ids;
    FOR (u, 1, n) { assert(1 <= e.bcc[u] && e.bcc[u] <= e.bccnt); ids.insert(e.bcc[u]); }
    assert((int)ids.size() == e.bccnt);                 // ids are exactly 1..bccnt
    FOR (u, 1, n) FOR (v, u + 1, n) {
      bool same = conn(u, v, -1);
      FOR (i, 0, (int)E.size() - 1) same = same && conn(u, v, i);
      assert(same == (e.bcc[u] == e.bcc[v]));
    }
    cases++;
  }
  printf("EBCC: %d multigraphs (n<=9, parallel edges, self-loops) vs edge-deletion brute force\n", cases);
}
