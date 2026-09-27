#include "prelude.h"
#include "stress.h"
const int N = 200;
#include "3_Data_Structure/Centroid_Decomposition.cpp"

Cent_Dec cd; // one global object reused across test cases, as in a multi-test problem
vector<pll> adj[N];
ll D[N][N];
void go(int s, int u, int f, ll d) { D[s][u] = d; for (auto [v, w] : adj[u]) if (v != f) go(s, v, u, d + w); }
int main() {
  int cases = 0, ops = 0;
  for (int it = 0; it < 3000; it++) {
    int n = rnd(1, it % 5 ? 30 : N - 1);
    cd.init(n);
    FOR (i, 1, n) adj[i].clear();
    FOR (i, 2, n) {
      int p = rnd(0, 1) ? rnd(max(1, i - 2), i - 1) : rnd(1, i - 1), w = rnd(0, 1000);
      cd.add_edge(p, i, w), adj[p].pb(i, w), adj[i].pb(p, w);
    }
    FOR (i, 1, n) go(i, i, 0, 0);
    cd.build();
    vector<int> cnt(n + 1);
    FOR (q, 1, 3 * n) {
      int u = rnd(1, n);
      if (rnd(0, 1)) cd.modify(u), cnt[u]++; // marking twice counts twice
      else {
        ll want = 0;
        FOR (v, 1, n) want += cnt[v] * D[u][v];
        ll got = cd.query(u);
        if (got != want) printf("case %d n=%d u=%d want %lld got %lld\n", it, n, u, want, got), fflush(stdout), assert(0);
      }
      ops++;
    }
    cases++;
  }
  printf("%d trees (n<=%d, one reused object), %d ops\n", cases, N - 1, ops);
}
