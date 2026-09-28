#include "prelude.h"
#include "stress.h"
const int MAXN = 20;
vector<int> adj[MAXN];
#include "2_Graph/SCC.cpp"
// u, v share an SCC iff each reaches the other (Floyd-Warshall closure).
// Also: ids are 1..sccnt, numbered sink first (every edge between SCCs goes
// from a larger id to a smaller one, which 2SAT relies on), and build_adj()
// holds exactly the edges between different SCCs.
int main() {
  int cases = 0;
  FOR (it, 1, 30000) {
    int n = rnd(1, 10), m = rnd(0, 25);
    vector<pair<int, int>> E;
    FOR (i, 1, n) adj[i].clear();
    FOR (i, 1, m) {                                   // self-loops and parallel
      int a = rnd(1, n), b = rnd(1, 5) == 1 ? a : rnd(1, n); // edges allowed
      E.pb(a, b), adj[a].pb(b);
    }
    vector<vector<int>> r(n + 1, vector<int>(n + 1));
    FOR (i, 1, n) r[i][i] = 1;
    for (auto [a, b] : E) r[a][b] = 1;
    FOR (k, 1, n) FOR (i, 1, n) FOR (j, 1, n) r[i][j] |= r[i][k] && r[k][j];
    SCC s(n); s.work(); s.build_adj();
    set<int> ids;
    FOR (u, 1, n) { assert(1 <= s.scc[u] && s.scc[u] <= s.sccnt); ids.insert(s.scc[u]); }
    assert((int)ids.size() == s.sccnt);
    FOR (u, 1, n) FOR (v, 1, n) assert((r[u][v] && r[v][u]) == (s.scc[u] == s.scc[v]));
    multiset<pair<int, int>> want, got;
    for (auto [a, b] : E) if (s.scc[a] != s.scc[b]) {
      assert(s.scc[a] > s.scc[b]);                    // sink first
      want.insert({s.scc[a], s.scc[b]});
    }
    FOR (c, 1, s.sccnt) for (int d : s.scc_adj[c]) got.insert({c, d});
    assert(want == got);
    cases++;
  }
  printf("SCC: %d digraphs (n<=10, self-loops, parallel edges) vs reachability closure\n", cases);
}
