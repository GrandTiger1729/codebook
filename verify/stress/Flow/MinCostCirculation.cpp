#include "prelude.h"
#include "stress.h"
const int N = 105;
const ll INF = 1e18;
#include "4_Flow_Matching/MinCostCirculation.cpp"

struct E { int u, v; ll cap, cost; };
// reference: cycle cancelling with Bellman-Ford on the residual graph
ll cycleCancel(int n, const vector<E> &es) {
  int m = es.size();
  vector<ll> f(m, 0);
  while (true) {
    // residual arcs: 2i forward, 2i+1 backward
    vector<ll> d(n, 0); vector<int> pe(n, -1);
    int x = -1;
    FOR (round, 1, n) {
      x = -1;
      FOR (i, 0, m - 1) {
        auto &e = es[i];
        if (f[i] < e.cap && d[e.u] + e.cost < d[e.v]) d[e.v] = d[e.u] + e.cost, pe[e.v] = 2 * i, x = e.v;
        if (f[i] > 0 && d[e.v] - e.cost < d[e.u]) d[e.u] = d[e.v] - e.cost, pe[e.u] = 2 * i + 1, x = e.u;
      }
    }
    if (x == -1) break;
    auto pred = [&](int y) { return pe[y] % 2 ? es[pe[y] / 2].v : es[pe[y] / 2].u; };
    FOR (k, 1, n) x = pred(x); // now surely on the cycle
    // collect cycle
    vector<int> cyc;
    int y = x;
    do {
      cyc.pb(pe[y]);
      y = pred(y);
    } while (y != x);
    ll b = LLONG_MAX;
    for (int a : cyc) b = min(b, a % 2 ? f[a / 2] : es[a / 2].cap - f[a / 2]);
    for (int a : cyc) f[a / 2] += a % 2 ? -b : b;
  }
  ll c = 0;
  FOR (i, 0, m - 1) c += f[i] * es[i].cost;
  return c;
}
ll brute(int n, const vector<E> &es) {
  int m = es.size();
  vector<ll> f(m, 0);
  ll best = LLONG_MAX;
  while (true) {
    vector<ll> bal(n, 0);
    FOR (i, 0, m - 1) bal[es[i].u] -= f[i], bal[es[i].v] += f[i];
    if (count(bal.begin(), bal.end(), 0) == n) {
      ll c = 0;
      FOR (i, 0, m - 1) c += f[i] * es[i].cost;
      best = min(best, c);
    }
    int i = 0;
    while (i < m && f[i] == es[i].cap) f[i] = 0, i++;
    if (i == m) break;
    f[i]++;
  }
  return best;
}
ll run(int n, const vector<E> &es, int mxlg) {
  mcmf.init(n);
  for (auto &e : es) mcmf.add_edge(e.u, e.v, e.cap, e.cost);
  mcmf.solve(mxlg);
  ll c = 0;
  vector<ll> bal(n, 0);
  FOR (u, 0, n - 1) for (auto &e : mcmf.G[u]) {
    if (e.fcap > 0) { // forward edges (reverse ones have fcap 0)
      assert(0 <= e.flow && e.flow <= e.fcap && e.cap == e.fcap);
      c += e.flow * e.cost, bal[u] -= e.flow, bal[e.to] += e.flow;
    }
  }
  FOR (v, 0, n - 1) assert(bal[v] == 0);
  return c;
}
int main() {
  int cases = 0;
  FOR (it, 1, 20000) { // tiny, vs enumeration
    int n = rnd(1, 4), m = rnd(1, 5);
    vector<E> es(m);
    for (auto &e : es) e = {(int)rnd(0, n - 1), (int)rnd(0, n - 1), rnd(0, 3), rnd(-5, 5)};
    ll b = brute(n, es);
    assert(run(n, es, 2) == b);
    assert(cycleCancel(n, es) == b); // sanity for the reference
    cases++;
  }
  FOR (it, 1, 3000) { // medium, vs cycle cancelling; big caps, mxlg over-estimated sometimes
    int n = rnd(2, 12), m = rnd(1, 30), lg = rnd(0, 20);
    vector<E> es(m);
    for (auto &e : es) e = {(int)rnd(0, n - 1), (int)rnd(0, n - 1), rnd(0, (1ll << lg) - 1 + (lg == 0)), rnd(-1000, 1000)};
    ll mx = 0; for (auto &e : es) mx = max(mx, e.cap);
    int mxlg = mx ? __lg(mx) : 0;
    assert(run(n, es, mxlg + rnd(0, 2)) == cycleCancel(n, es));
    cases++;
  }
  printf("MinCostCirculation: %d cases OK\n", cases);
}
