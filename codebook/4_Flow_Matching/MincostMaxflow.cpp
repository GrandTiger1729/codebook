#include <ext/pb_ds/priority_queue.hpp> // decrease-key
const ll INF = numeric_limits<ll>::max() / 4;
struct MinCostMaxFlow { // 0-base, need global N
  struct Edge {
    int from, to, rev;
    ll cap, cost, flow;
  } *par[N];
  vector<Edge> g[N];
  int n, vis[N];
  ll dis[N], pot[N];
  void init(int _n) {
    n = _n, fill_n(pot, n, 0);
    FOR (i, 0, n - 1) g[i].clear();
  }
  void add_edge(int a, int b, ll cap, ll cost) {
    if (a == b) return;
    g[a].pb(
      Edge{a, b, (int)g[b].size(), cap, cost, 0});
    g[b].pb(
      Edge{b, a, (int)g[a].size() - 1, 0, -cost, 0});
  }
  void path(int s) { // dijkstra on reduced costs
    fill_n(vis, n, 0), fill_n(dis, n, INF);
    dis[s] = 0;
    __gnu_pbds::priority_queue<pair<ll, int>> pq;
    vector<decltype(pq)::point_iterator> it(n);
    pq.push({0, s});
    while (!pq.empty()) {
      s = pq.top().S, pq.pop(), vis[s] = 1;
      ll d = dis[s] + pot[s];
      for (Edge &e : g[s])
        if (!vis[e.to]) {
          ll v = d - pot[e.to] + e.cost;
          if (e.cap - e.flow > 0 && v < dis[e.to]) {
            dis[e.to] = v, par[e.to] = &e;
            if (it[e.to] == pq.end())
              it[e.to] = pq.push({-v, e.to});
            else pq.modify(it[e.to], {-v, e.to});
          }
        }
    }
    FOR (i, 0, n - 1)
      pot[i] = min(pot[i] + dis[i], INF);
  }
  pair<ll, ll> maxflow(int s, int t) {
    ll flow = 0, cost = 0;
    while (path(s), vis[t]) {
      ll f = INF;
      for (Edge *e = par[t]; e; e = par[e->from])
        f = min(f, e->cap - e->flow);
      flow += f;
      for (Edge *e = par[t]; e; e = par[e->from])
        e->flow += f, g[e->to][e->rev].flow -= f;
    }
    FOR (i, 0, n - 1)
      for (Edge &e : g[i]) cost += e.cost * e.flow;
    return {flow, cost / 2};
  }
  void setpi(int s) { // iff some cost is negative
    fill_n(pot, n, INF), pot[s] = 0;
    int it = n, ch = 1;
    ll v;
    while (ch-- && it--)
      FOR (i, 0, n - 1)
        if (pot[i] != INF)
          for (Edge &e : g[i])
            if (e.cap)
              if ((v = pot[i] + e.cost) < pot[e.to])
                pot[e.to] = v, ch = 1;
    assert(it >= 0); // negative cost cycle
  }
};
