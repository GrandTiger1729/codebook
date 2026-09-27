#include "prelude.h"
#include "stress.h"
const int N = 8;
const ll INF = 1e18;
#include "2_Graph/MinimumMeanCycle.cpp"

MinimumMeanCycle mmc;
int n;
ll bn, bd; // best mean bn/bd, bd=-1: none
bool vis[N];
void go(int s, int u, ll w, ll len) {
  FOR (v, s, n - 1) if (road[u][v] < INF) {
    if (v == s) {
      ll W = w + road[u][v], Ln = len + 1;
      if (bd == -1 || W * bd < bn * Ln) bn = W, bd = Ln;
    } else if (!vis[v]) vis[v] = 1, go(s, v, w + road[u][v], len + 1), vis[v] = 0;
  }
}
int main() {
  int cases = 0;
  for (int it = 0; it < 20000; it++) {
    n = rnd(1, N);
    int dens = rnd(0, 100), W = rnd(1, 3) == 1 ? 3 : 1000;
    bool loops = rnd(0, 1);
    FOR (i, 0, n - 1) FOR (j, 0, n - 1)
      road[i][j] = ((i != j || loops) && rnd(0, 99) < dens) ? rnd(1, W) : INF;
    bd = -1;
    FOR (s, 0, n - 1) vis[s] = 1, go(s, s, 0, 0), vis[s] = 0;
    pll want(-1, -1);
    if (bd != -1) { ll g = __gcd(bn, bd); want = pll(bn / g, bd / g); }
    mmc.init(n);
    pll got = mmc.solve();
    if (got != want) {
      printf("n=%d want %lld/%lld got %lld/%lld\n", n, want.F, want.S, got.F, got.S);
      FOR (i, 0, n - 1) { FOR (j, 0, n - 1) printf("%lld ", road[i][j] >= INF ? -1 : road[i][j]); puts(""); }
      fflush(stdout), assert(0);
    }
    cases++;
  }
  printf("%d cases, n<=%d, positive weights\n", cases, N);
}
