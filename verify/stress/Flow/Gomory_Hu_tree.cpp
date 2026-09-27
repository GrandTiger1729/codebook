#include "prelude.h"
#include "stress.h"
const int MAXN = 8, INF = 1e9; // tight on purpose: n reaches MAXN
#include "4_Flow_Matching/Dinic.cpp" // commented out of content.tex; supplies MaxFlow
vector<array<int, 3>> tree;
void add_edge(int u, int v, int w) { tree.push_back({u, v, w}); } // the tree edge the template emits
#include "4_Flow_Matching/Gomory_Hu_tree.cpp"

int main() {
  int cases = 0;
  FOR (it, 1, 6000) {
    int n = rnd(2, 8), m = rnd(0, 14), W = rnd(0, 1) ? 1 : 20;
    vector<vector<int>> w(n, vector<int>(n, 0));
    Dinic.init(n);
    FOR (k, 1, m) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1), c = rnd(0, W);
      if (u == v) continue;
      w[u][v] += c, w[v][u] += c;
      Dinic.add_edge(u, v, c), Dinic.add_edge(v, u, c); // undirected
    }
    tree.clear();
    GomoryHu(n);
    assert((int)tree.size() == n - 1);
    // brute min cut for every pair
    vector<vector<int>> cut(n, vector<int>(n, INT_MAX));
    FOR (mask, 1, (1 << n) - 2) {
      int c = 0;
      FOR (i, 0, n - 1) FOR (j, 0, n - 1) if ((mask >> i & 1) && !(mask >> j & 1)) c += w[i][j];
      FOR (i, 0, n - 1) FOR (j, 0, n - 1) if ((mask >> i & 1) && !(mask >> j & 1)) cut[i][j] = min(cut[i][j], c);
    }
    // min edge on tree path
    vector<vector<pair<int, int>>> adj(n);
    for (auto [u, v, c] : tree) {
      assert(0 <= u && u < n && 0 <= v && v < n && u != v);
      adj[u].pb(v, c), adj[v].pb(u, c);
    }
    FOR (src, 0, n - 1) {
      vector<int> mn(n, -1);
      mn[src] = INT_MAX;
      vector<int> st = {src};
      while (!st.empty()) {
        int u = st.back(); st.pop_back();
        for (auto [v, c] : adj[u]) if (mn[v] == -1) mn[v] = min(mn[u], c), st.pb(v);
      }
      FOR (j, 0, n - 1) if (j != src) assert(mn[j] != -1 && mn[j] == cut[src][j]); // connected tree + flow-equivalent
    }
    cases++;
  }
  printf("GomoryHu: %d graphs OK\n", cases);
}
