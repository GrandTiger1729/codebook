#include "prelude.h"
#include "stress.h"
const int N = 10;
#include "4_Flow_Matching/MincostMaxflow.cpp" // defines INF itself
MinCostMaxFlow mf;
struct E { int u, v; ll cap, cost; };
// reference: successive shortest paths with SPFA on the residual graph
pair<ll, ll> ref(int n, const vector<E> &es, int s, int t) {
  int m = es.size();
  vector<ll> f(m, 0);
  ll flow = 0, cost = 0;
  while (true) {
    vector<ll> d(n, INF); vector<int> pe(n, -1), inq(n, 0);
    deque<int> q{s}; d[s] = 0, inq[s] = 1;
    while (!q.empty()) {
      int x = q.front(); q.pop_front(); inq[x] = 0;
      FOR (i, 0, m - 1) {
        auto &e = es[i];
        auto relax = [&](int a, int b, ll c, int id) {
          if (a == x && d[a] + c < d[b]) {
            d[b] = d[a] + c, pe[b] = id;
            if (!inq[b]) inq[b] = 1, q.pb(b);
          }
        };
        if (f[i] < e.cap) relax(e.u, e.v, e.cost, 2 * i);
        if (f[i] > 0) relax(e.v, e.u, -e.cost, 2 * i + 1);
      }
    }
    if (d[t] >= INF) break;
    ll aug = INF;
    for (int y = t; y != s;) {
      int id = pe[y]; auto &e = es[id / 2];
      aug = min(aug, id % 2 ? f[id / 2] : e.cap - f[id / 2]);
      y = id % 2 ? e.v : e.u;
    }
    for (int y = t; y != s;) {
      int id = pe[y]; auto &e = es[id / 2];
      f[id / 2] += id % 2 ? -aug : aug;
      y = id % 2 ? e.v : e.u;
    }
    flow += aug, cost += aug * d[t];
  }
  return {flow, cost};
}
// Random digraphs (n <= 8, parallel / antiparallel edges, self-loops, caps
// up to 6, some cap-0 edges). Half the cases have negative costs built as
// c + p[u] - p[v] with c >= 0 (so no negative cycle) and call setpi(s)
// first, as the template says to; the other half have costs >= 0 only.
int main() {
  int cases = 0, neg = 0;
  FOR (it, 1, 4000) {
    int n = rnd(2, 8), m = rnd(0, 20), s = rnd(0, n - 1), t = rnd(0, n - 1);
    while (t == s) t = rnd(0, n - 1);
    bool ng = it % 2;
    vector<ll> p(n);
    for (ll &x : p) x = ng ? rnd(0, 25) : 0;
    vector<E> es;
    mf.init(n);
    bool anyneg = false;
    FOR (i, 1, m) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1);
      ll cap = rnd(0, 6), c = rnd(0, 10) + p[u] - p[v];
      mf.add_edge(u, v, cap, c);
      if (u != v) es.push_back({u, v, cap, c}), anyneg |= c < 0;
    }
    if (ng) mf.setpi(s);
    neg += anyneg;
    auto want = ref(n, es, s, t);
    auto got = mf.maxflow(s, t);
    assert(got == want);
    cases++;
  }
  printf("MincostMaxflow: %d digraphs (n<=8, m<=20; %d with negative costs via setpi) vs SPFA successive shortest paths\n",
    cases, neg);
}
