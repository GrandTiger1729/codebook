#include "prelude.h"
#include "stress.h"
const int N = 300, SMALL = 8;
const ll INF = 1e18;
#include "2_Graph/MinimumMeanCycle.cpp"
// Small: n <= 8, exact enumeration of simple cycles. Large: n <= 300, weights in [1, 1e9]
// (the template needs weights > 0), vs min over q = 1..n of the minimum closed walk of
// exactly q edges divided by q (a closed walk splits into simple cycles, so the minimum mean
// over closed walks of length <= n is the minimum mean cycle). Values <= n * 1e9, so the
// fraction comparisons (<= 3e11 * 300) stay far inside ll.

MinimumMeanCycle mmc;
int n;
ll bn, bd; // best mean bn/bd, bd=-1: none
bool vis[SMALL];
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
    n = rnd(1, SMALL);
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
  int big = 0;
  FOR (it, 1, 200) {
    bool dense = it % 3 == 0;
    n = dense ? rnd(1, 80) : it % 10 ? rnd(1, N) : N;
    ll W = it % 4 ? 1000000000 : rnd(1, 1000);
    FOR (i, 0, n - 1) FOR (j, 0, n - 1) road[i][j] = INF;
    if (dense) {
      FOR (i, 0, n - 1) FOR (j, 0, n - 1) if (rnd(0, 99) < 60 && (i != j || it % 2)) road[i][j] = rnd(1, W);
    } else { // sparse: ~2-3 out-edges per vertex, sometimes a long cheap cycle 0 -> 1 -> ... -> 0
      FOR (e, 1, rnd(n, 3 * n)) road[rnd(0, n - 1)][rnd(0, n - 1)] = rnd(1, W);
      if (it % 5 == 0) FOR (i, 0, n - 1) road[i][(i + 1) % n] = rnd(1, 3);
    }
    // cur[v] after q steps: minimum weight of a walk s -> v with exactly q edges
    ll bn = -1, bd = -1;
    static ll cur[N], nxt[N];
    static array<ll, 3> E[N * N]; // O(n^2 m) total; plain arrays (debug-mode vectors are slow)
    int m = 0;
    FOR (u, 0, n - 1) FOR (v, 0, n - 1) if (road[u][v] < INF) E[m++] = {u, v, road[u][v]};
    FOR (s, 0, n - 1) {
      fill(cur, cur + n, INF), cur[s] = 0;
      FOR (q, 1, n) {
        fill(nxt, nxt + n, INF);
        FOR (e, 0, m - 1) if (auto [u, v, w] = E[e]; cur[u] < INF) nxt[v] = min(nxt[v], cur[u] + w);
        copy(nxt, nxt + n, cur);
        if (cur[s] < INF && (bd == -1 || cur[s] * bd < bn * q)) bn = cur[s], bd = q;
      }
    }
    pll want(-1, -1);
    if (bd != -1) { ll g = __gcd(bn, bd); want = pll(bn / g, bd / g); }
    mmc.init(n);
    assert(mmc.solve() == want);
    cases++, big++;
  }
  printf("%d cases (%d with n<=%d, weights<=1e9), positive weights\n", cases, big, N);
}
