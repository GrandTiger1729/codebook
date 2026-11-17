// The "negative cycles" reduction in Flow Models (Model.tex), run on MincostMaxflow:
// saturate every negative edge, then all costs are >= 0 and no setpi() is needed.
//  (a) t->s with (inf, 0): min cost of an s-t flow of any value
//  (b) t->s with (U, -M), saturated as well: min cost maximum flow
// Both vs enumerating every integer flow on small graphs with negative cycles.
#include "prelude.h"
#include "stress.h"
const int N = 10;
#include "4_Flow_Matching/MincostMaxflow.cpp"
MinCostMaxFlow mf;
struct E { int u, v; ll cap, cost; };

int main() {
  int cases = 0, negcyc = 0;
  FOR (it, 1, 20000) {
    int n = rnd(2, 5), m = rnd(1, 6), s = 0, t = n - 1;
    vector<E> es(m);
    for (auto &e : es) e = {(int)rnd(0, n - 1), (int)rnd(0, n - 1), rnd(0, 2), rnd(-5, 5)};
    // brute force: every f with 0 <= f_e <= cap_e and conservation except at s, t
    ll bestAny = INF, maxF = -1, bestMax = INF;
    vector<ll> f(m, 0);
    while (true) {
      vector<ll> ex(n, 0); ll c = 0;
      FOR (i, 0, m - 1) if (es[i].u != es[i].v) ex[es[i].v] += f[i], ex[es[i].u] -= f[i];
      FOR (i, 0, m - 1) c += f[i] * es[i].cost;
      bool ok = ex[s] <= 0;
      FOR (v, 0, n - 1) if (v != s && v != t && ex[v]) ok = false;
      if (ok) {
        ll F = ex[t];
        bestAny = min(bestAny, c);
        if (F > maxF) maxF = F, bestMax = c;
        else if (F == maxF) bestMax = min(bestMax, c);
      }
      int i = 0;
      while (i < m && f[i] == es[i].cap) f[i++] = 0;
      if (i == m) break;
      f[i]++;
    }
    { // does a negative cycle exist? (Bellman-Ford on the edges with cap > 0)
      vector<ll> d(n, 0); bool ch = false;
      FOR (r, 1, n) { ch = false;
        for (auto &e : es) if (e.cap && d[e.u] + e.cost < d[e.v]) d[e.v] = d[e.u] + e.cost, ch = true; }
      negcyc += ch;
    }
    ll sumAbs = 1, U = 0;
    for (auto &e : es) sumAbs += abs(e.cost);
    for (auto &e : es) if (e.u == s && e.v != s) U += e.cap;
    FOR (variant, 0, 1) {
      int S = n, T = n + 1;
      mf.init(n + 2);
      vector<ll> d(n, 0); ll ans = 0;
      auto add = [&](int u, int v, ll cap, ll c) {
        if (u == v) { if (c < 0) ans += cap * c; return; } // a negative self-loop is always used
        if (c < 0) ans += cap * c, mf.add_edge(v, u, cap, -c), d[v] += cap, d[u] -= cap;
        else mf.add_edge(u, v, cap, c);
      };
      for (auto &e : es) add(e.u, e.v, e.cap, e.cost);
      int rev = -1; // index of the reversed t->s edge in mf.g[s] (variant b)
      if (variant == 0) add(t, s, INF, 0);
      else rev = mf.g[s].size(), add(t, s, U, -sumAbs);
      ll need = 0;
      FOR (v, 0, n - 1) {
        if (d[v] > 0) mf.add_edge(S, v, d[v], 0), need += d[v];
        if (d[v] < 0) mf.add_edge(v, T, -d[v], 0);
      }
      auto [fl, c] = mf.maxflow(S, T);
      assert(fl == need); // always saturates
      ans += c;
      if (variant == 0) assert(ans == bestAny);
      else {
        ll F = U - mf.g[s][rev].flow;
        assert(F == maxF && ans + sumAbs * F == bestMax);
      }
    }
    cases++;
  }
  printf("MCMF negative-cycle reduction ok: %d graphs (%d with a negative cycle), both variants\n", cases, negcyc);
}
