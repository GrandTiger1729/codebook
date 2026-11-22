// Minimum_Arborescence_fast.cpp vs brute force (every choice of one in-edge per
// non-root vertex) on small random multigraphs, including ones with no arborescence
// and n = 1. Checks: an answer exists iff the result has n - 1 edges, and then the
// edges form an arborescence rooted at root with the brute-force minimum weight.
#include "prelude.h"
#include "stress.h"
#include "3_Data_Structure/DSU.cpp"
#include "3_Data_Structure/min_heap.cpp"
#include "2_Graph/Minimum_Arborescence_fast.cpp"
// min_heap never frees the nodes still in a heap (no destructor): fine in a contest
extern "C" const char *__asan_default_options() { return "detect_leaks=0"; }
int main() {
  int cases = 0, none = 0, ones = 0;
  FOR (it, 1, 30000) {
    int n = rnd(1, 6), root = rnd(0, n - 1), m = rnd(0, 12);
    vector<E> e;
    FOR (i, 1, m) e.pb(E{rnd(0, n - 1), rnd(0, n - 1), (ll)rnd(-5, 20)});
    // brute force
    vector<vector<int>> in(n);
    FOR (i, 0, m - 1) if (e[i].s != e[i].t && e[i].t != root) in[e[i].t].pb(i);
    ll best = LLONG_MAX;
    vector<int> pick(n, -1);
    function<void(int, ll)> go = [&](int v, ll w) {
      if (v == n) {
        FOR (u, 0, n - 1) { // every vertex reaches root
          int x = u, steps = 0;
          while (x != root && steps <= n) x = e[pick[x]].s, steps++;
          if (x != root) return;
        }
        best = min(best, w);
        return;
      }
      if (v == root) return go(v + 1, w);
      for (int i : in[v]) pick[v] = i, go(v + 1, w + e[i].w);
    };
    go(0, 0);
    auto ans = dmst(e, n, root);
    if (best == LLONG_MAX) {
      assert((int)ans.size() != n - 1); // no answer: detectable by size
      none++;
    } else {
      assert((int)ans.size() == n - 1);
      vector<int> par(n, -1), seen(m);
      ll w = 0;
      for (int i : ans) {
        assert(0 <= i && i < m && !seen[i]++);
        assert(e[i].t != root && par[e[i].t] == -1 && e[i].s != e[i].t);
        par[e[i].t] = e[i].s, w += e[i].w;
      }
      FOR (u, 0, n - 1) {
        int x = u, steps = 0;
        while (x != root && steps <= n) x = par[x], steps++;
        assert(x == root);
      }
      assert(w == best);
      ones += n == 1;
    }
    cases++;
  }
  printf("Arborescence ok: %d graphs (%d with no answer, %d with n = 1)\n", cases, none, ones);
}
