#include "prelude.h"
#include "stress.h"
const int N = 105, INF = 1e9;
#include "4_Flow_Matching/BoundedFlow.cpp"

// LOJ 116/117 usage: x = solve(s,t) is some feasible s-t flow (-1 = none);
// max = x + maxflow(s,t), min = x - maxflow(t,s).
// Convention (from the INF t->s edge): "feasible" means a flow of value >= 0 exists;
// the min flow is then the minimum over all feasible flows, which may be negative.
struct E { int u, v, l, r; };
BoundedFlow bf; // one global object reused across cases with varying n (as in contest)

void build(int n, const vector<E> &es) {
  bf.init(n);
  for (auto &e : es) bf.add_edge(e.u, e.v, e.l, e.r);
}
int main() {
  int cases = 0, feasCnt = 0;
  FOR (it, 1, 30000) {
    int n = rnd(2, 5), m = rnd(1, 6), s = rnd(0, n - 1), t = rnd(0, n - 2);
    if (t >= s) t++;
    vector<E> es(m);
    for (auto &e : es) {
      e.u = rnd(0, n - 1), e.v = rnd(0, n - 1);
      e.l = rnd(0, 2), e.r = e.l + rnd(0, 2);
    }
    // brute force: enumerate integer flows
    bool feas = false, circ = false;
    int mx = INT_MIN, mn = INT_MAX;
    vector<int> f(m);
    for (int i = 0; i < m; i++) f[i] = es[i].l;
    while (true) {
      vector<int> bal(n, 0);
      FOR (i, 0, m - 1) bal[es[i].u] -= f[i], bal[es[i].v] += f[i];
      bool ok = true;
      FOR (v, 0, n - 1) if (v != s && v != t && bal[v]) ok = false;
      if (ok && -bal[s] >= 0) feas = true;
      if (ok) mx = max(mx, -bal[s]), mn = min(mn, -bal[s]);
      if (ok && !bal[s] && !bal[t]) circ = true;
      int i = 0;
      while (i < m && f[i] == es[i].r) f[i] = es[i].l, i++;
      if (i == m) break;
      f[i]++;
    }
    // circulation feasibility: solve()
    build(n, es);
    assert(bf.solve() == circ);
    // max flow
    build(n, es);
    int x = bf.solve(s, t);
    assert((x != -1) == feas);
    if (feas) {
      int got = x + bf.maxflow(s, t);
      assert(got == mx);
      // the flow on the edges must be valid
      vector<int> bal(n, 0);
      FOR (u, 0, n - 1) for (auto &e : bf.G[u]) if (e.cap) { // forward edges
        assert(0 <= e.flow && e.flow <= e.cap);
        bal[u] -= e.flow, bal[e.to] += e.flow;
      }
      FOR (v, 0, n - 1) if (v != s && v != t) assert(!bal[v]);
      assert(-bal[s] == mx);
      // min flow
      build(n, es);
      x = bf.solve(s, t);
      assert(x - bf.maxflow(t, s) == mn);
      feasCnt++;
    }
    cases++;
  }
  printf("BoundedFlow: %d cases OK (%d feasible)\n", cases, feasCnt);
}
