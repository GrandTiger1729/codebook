#include "prelude.h"
#include "stress.h"
const int N = 14;
// fwt: the OR transform from 7_Polynomial/Fast_Walsh_Transform.cpp, typed as a contestant would
void fwt(int *a, int n, int op) { //or
  for (int L = 2; L <= n; L <<= 1)
    for (int i = 0; i < n; i += L)
      FOR (j, i, j + (L >> 1) - 1)
        a[j + (L >> 1)] += a[j] * op;
}
#include "2_Graph/Minimum_Clique_Cover.cpp"

Clique_Cover cc;
int adj[N], n, dp[1 << N];
bool cl[1 << N];
int main() {
  int cases = 0;
  for (int it = 0; it < 3000; it++) {
    n = rnd(1, it % 10 ? 10 : N);
    int dens = rnd(0, 100);
    cc.init(n);
    FOR (i, 0, n - 1) adj[i] = 1 << i;
    FOR (i, 0, n - 1) FOR (j, i + 1, n - 1) if (rnd(0, 99) < dens)
      cc.add_edge(i, j), adj[i] |= 1 << j, adj[j] |= 1 << i;
    cl[0] = 1;
    FOR (S, 1, (1 << n) - 1) { int v = __builtin_ctz(S); cl[S] = cl[S ^ 1 << v] && (adj[v] & S) == S; }
    dp[0] = 0;
    FOR (S, 1, (1 << n) - 1) {
      int v = __builtin_ctz(S), r = S ^ 1 << v; dp[S] = 1e9;
      for (int T = r;; T = (T - 1) & r) { // subsets containing v
        if (cl[T | 1 << v]) dp[S] = min(dp[S], dp[r & ~T] + 1);
        if (!T) break;
      }
    }
    int got = cc.solve();
    if (got != dp[(1 << n) - 1]) printf("n=%d want %d got %d\n", n, dp[(1 << n) - 1], got), fflush(stdout), assert(0);
    cases++;
  }
  printf("%d graphs, n<=%d\n", cases, N);
}
