typedef pair<ll, ll> pll;
struct Cent_Dec { // 1-base
  vector<pll> G[N];
  pll info[N]; // store info. of itself
  pll upinfo[N]; // store info. of climbing up
  int n, pa[N], layer[N], sz[N], done[N];
  ll dis[__lg(N) + 1][N];
  void init(int _n) {
    n = _n, layer[0] = -1;
    fill_n(pa + 1, n, 0), fill_n(done + 1, n, 0);
    FOR (i, 1, n) G[i].clear();
  }
  void add_edge(int a, int b, int w) {
    G[a].pb(pll(b, w)), G[b].pb(pll(a, w));
  }
  void get_cent(
    int u, int f, int &mx, int &c, int num) {
    int mxsz = 0;
    sz[u] = 1;
    for (pll e : G[u])
      if (!done[e.F] && e.F != f) {
        get_cent(e.F, u, mx, c, num);
        sz[u] += sz[e.F], mxsz = max(mxsz, sz[e.F]);
      }
    if (mx > max(mxsz, num - sz[u]))
      mx = max(mxsz, num - sz[u]), c = u;
  }
  void dfs(int u, int f, ll d, int org) {
    // if required, add self info or climbing info
    dis[layer[org]][u] = d;
    for (pll e : G[u])
      if (!done[e.F] && e.F != f)
        dfs(e.F, u, d + e.S, org);
  }
  int cut(int u, int f, int num) {
    int mx = 1e9, c = 0, lc;
    get_cent(u, f, mx, c, num);
    done[c] = 1, pa[c] = f, layer[c] = layer[f] + 1;
    for (pll e : G[c])
      if (!done[e.F]) {
        if (sz[e.F] > sz[c])
          lc = cut(e.F, c, num - sz[c]);
        else lc = cut(e.F, c, sz[e.F]);
        upinfo[lc] = pll(), dfs(e.F, c, e.S, c);
      }
    return done[c] = 0, c;
  }
  void build() { cut(1, 0, n); }
  void modify(int u) {
    for (int a = u, ly = layer[a]; a;
         a = pa[a], --ly) {
      info[a].F += dis[ly][u], ++info[a].S;
      if (pa[a])
        upinfo[a].F += dis[ly - 1][u], ++upinfo[a].S;
    }
  }
  ll query(int u) {
    ll rt = 0;
    for (int a = u, ly = layer[a]; a;
         a = pa[a], --ly) {
      rt += info[a].F + info[a].S * dis[ly][u];
      if (pa[a])
        rt -=
          upinfo[a].F + upinfo[a].S * dis[ly - 1][u];
    }
    return rt;
  }
};
