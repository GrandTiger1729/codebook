#include "prelude.h"
#include "stress.h"
const int N = 64;
#include "2_Graph/Maximum_Clique_Dyn.cpp"

MaxClique mc;
ll adj[N];
int best, n;
void bk(ll R, int rs, ll P) { // plain branch & bound
  if (!P) { best = max(best, rs); return; }
  if (rs + __builtin_popcountll(P) <= best) return;
  int v = __builtin_ctzll(P);
  bk(R | 1LL << v, rs + 1, P & adj[v]);
  bk(R, rs, P & ~(1LL << v));
}
int main() {
  int cases = 0;
  for (int it = 0; it < 6000; it++) {
    n = it % 3 ? rnd(0, 16) : rnd(17, 60);
    int dens = rnd(0, 100);
    mc.init(n);
    FOR (i, 0, n - 1) adj[i] = 0;
    FOR (i, 0, n - 1) FOR (j, i + 1, n - 1) if (rnd(0, 99) < dens)
      mc.add_edge(i, j), adj[i] |= 1LL << j, adj[j] |= 1LL << i;
    best = 0, bk(0, 0, n ? (n == 64 ? -1 : (1LL << n) - 1) : 0);
    int got = mc.solve();
    assert(got == best);
    FOR (a, 0, got - 1) {
      assert(0 <= mc.sol[a] && mc.sol[a] < n);
      FOR (b, 0, a - 1) assert(adj[mc.sol[a]] >> mc.sol[b] & 1);
    }
    cases++;
  }
  printf("%d graphs, n<=60, N=%d, answer + certificate checked\n", cases, N);
}
