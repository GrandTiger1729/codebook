// st_numbering on random multigraphs: whenever G + (s, t) is biconnected (checked by
// brute force: connected after deleting any single vertex, n >= 3), the result must be a
// permutation with s first, t last, and every other vertex having an earlier and a later
// neighbour. n = 2 (s, t only) is checked too.
#include "prelude.h"
#include "stress.h"
#include "2_Graph/BipolarOrientation.cpp"
bool connectedWithout(const vector<vector<int>> &g, int del) {
  int n = g.size(), st = del == 0 ? 1 : 0, cnt = 0;
  vector<int> vis(n, 0); vis[st] = 1; vector<int> q{st};
  while (!q.empty()) { int u = q.back(); q.pop_back(); cnt++;
    for (int v : g[u]) if (v != del && !vis[v]) vis[v] = 1, q.pb(v); }
  return cnt == n - (del >= 0);
}
int main() {
  int cases = 0, tried = 0;
  FOR (it, 1, 200000) {
    int n = rnd(2, it % 20 ? 8 : 60), m = rnd(n - 1, 3 * n);
    vector<vector<int>> g(n);
    FOR (i, 1, m) { int u = rnd(0, n - 1), v = rnd(0, n - 1); if (u != v) g[u].pb(v), g[v].pb(u); }
    int s = rnd(0, n - 1), t = rnd(0, n - 2); if (t >= s) t++;
    auto h = g; h[s].pb(t), h[t].pb(s);
    bool bic = connectedWithout(h, -1);
    if (n >= 3) FOR (d, 0, n - 1) bic = bic && connectedWithout(h, d);
    tried++;
    if (!bic) continue;
    vector<int> r = st_numbering(g, s, t), pos(n, -1);
    assert((int)r.size() == n && r[0] == s && r.back() == t);
    FOR (i, 0, n - 1) assert(pos[r[i]] < 0), pos[r[i]] = i;
    FOR (u, 0, n - 1) if (u != s && u != t) {
      bool lo = false, hi = false;
      for (int v : g[u]) lo |= pos[v] < pos[u], hi |= pos[v] > pos[u];
      assert(lo && hi);
    }
    cases++;
  }
  printf("st_numbering ok: %d biconnected cases out of %d random graphs (n<=60)\n", cases, tried);
}
