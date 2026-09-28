#include "prelude.h"
#include "stress.h"
const int N = 9, T = 5;
const ll INF = 1e18;
#include "2_Graph/MinimumSteinerTree.cpp"
SteinerTree st;
// Random undirected multigraphs (n <= 8, up to 5 terminals), optional vertex
// costs vcst, weights up to 30 or up to 1e9. Brute force: every vertex set
// containing the terminals, cost = sum vcst + MST of the induced subgraph
// (Prim on the lightest parallel edge). Also the reconstructed edge set ans
// must be a tree spanning the terminals whose weight + vertex costs equals
// the returned value.
int main() {
  int cases = 0, withv = 0, big = 0, infeas = 0;
  FOR (it, 1, 16000) {
    int n = rnd(1, 8), m = rnd(0, 14), t = rnd(1, min(n, T));
    bool vc = rnd(0, 1), bw = rnd(0, 2) == 0;
    ll W = bw ? 1000000000 : 30;
    withv += vc, big += bw;
    st.init(n);
    vector<ll> vcst(n, 0);
    if (vc) FOR (i, 0, n - 1) st.vcst[i] = vcst[i] = rnd(0, W);
    vector<array<ll, 3>> E; // u, v, w; id = index
    vector<vector<ll>> g(n, vector<ll>(n, INF));
    FOR (i, 0, m - 1) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1);
      while (n > 1 && v == u) v = rnd(0, n - 1);
      if (u == v) continue;
      ll w = rnd(1, W);
      int id = E.size();
      E.push_back({u, v, w});
      st.add_edge(u, v, w, id), st.add_edge(v, u, w, id);
      g[u][v] = g[v][u] = min(g[u][v], w);
    }
    vector<int> ter(n); iota(ter.begin(), ter.end(), 0);
    shuffle(ter.begin(), ter.end(), rng), ter.resize(t);
    int tm = 0;
    for (int x : ter) tm |= 1 << x;
    // brute force
    ll want = INF;
    FOR (msk, 0, (1 << n) - 1) if ((msk & tm) == tm) {
      vector<int> vs;
      FOR (i, 0, n - 1) if (msk >> i & 1) vs.pb(i);
      ll c = 0;
      for (int x : vs) c += vcst[x];
      vector<ll> d(n, INF); vector<int> in(n, 0);
      d[vs[0]] = 0;
      bool conn = true;
      FOR (k, 1, (int)vs.size()) {
        int b = -1;
        for (int x : vs) if (!in[x] && (b < 0 || d[x] < d[b])) b = x;
        if (d[b] >= INF) { conn = false; break; }
        in[b] = 1, c += d[b];
        for (int x : vs) if (!in[x]) d[x] = min(d[x], g[b][x]);
      }
      if (conn) want = min(want, c);
    }
    ll got = st.solve(ter);
    if (want >= INF) { assert(got >= INF); infeas++; cases++; continue; }
    assert(got == want);
    // reconstructed tree
    vector<int> ans = st.ans;
    set<int> V(ter.begin(), ter.end());
    ll c = 0;
    vector<int> f(n); iota(f.begin(), f.end(), 0);
    function<int(int)> fd = [&](int a) { return f[a] == a ? a : f[a] = fd(f[a]); };
    for (int id : ans) {
      assert(0 <= id && id < (int)E.size());
      auto [u, v, w] = E[id];
      c += w, V.insert(u), V.insert(v);
      assert(fd(u) != fd(v));               // no cycle
      f[fd(u)] = fd(v);
    }
    assert(ans.size() + 1 == V.size());     // hence a tree
    for (int x : ter) assert(fd(x) == fd(ter[0]));
    for (int x : V) c += vcst[x];
    assert(c == got);
    cases++;
  }
  printf("MinimumSteinerTree: %d graphs (n<=8, T<=5; %d with vcst, %d with W=1e9, %d infeasible) vs vertex-subset + Prim; ans tree checked\n",
    cases, withv, big, infeas);
}
