#include "../prelude.h"
const int N = 1005;   // 2*500 + 2
#include "../../codebook/4_Flow_Matching/MincostMaxflow.cpp"

// Assignment as min-cost max-flow: s -> left_i -> right_j -> t.
// a_ij can be negative; instead of setpi (Bellman-Ford, O(VE), far too slow at
// N=500) shift every cost by +SHIFT and subtract N*SHIFT from the answer --
// every feasible flow uses exactly N of these edges.
const ll SHIFT = 1000000000;

int main() {
  Waimai;
  int n; cin >> n;
  int s = 2 * n, t = 2 * n + 1;
  static MinCostMaxFlow mcmf;
  mcmf.init(2 * n + 2);
  for (int i = 0; i < n; i++) {
    mcmf.add_edge(s, i, 1, 0);
    mcmf.add_edge(n + i, t, 1, 0);
  }
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      ll a; cin >> a;
      mcmf.add_edge(i, n + j, 1, a + SHIFT);
    }
  auto [flow, cost] = mcmf.maxflow(s, t);
  assert(flow == n);
  cout << cost - (ll)n * SHIFT << '\n';
  for (int i = 0; i < n; i++)
    for (auto &e : mcmf.g[i])
      if (e.to >= n && e.to < 2 * n && e.flow > 0)
        { cout << e.to - n << " \n"[i + 1 == n]; break; }
}
