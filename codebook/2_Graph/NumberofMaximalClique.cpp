struct BronKerbosch { // 1-base
  // time ~ answer (~7e7/s); worst 3^(n/3): n = 48 in 0.6s
  int n, a[N], g[N][N];
  int cnt, all[N][N], some[N][N], none[N][N];
  void init(int _n) {
    n = _n;
    FOR (i, 1, n) FOR (j, 1, n) g[i][j] = 0;
  }
  void add_edge(int u, int v) {
    g[u][v] = g[v][u] = 1;
  }
  void dfs(int d, int an, int sn, int nn) {
    if (sn == 0 && nn == 0) ++cnt;
    int u = 0, best = -1;
    FOR (k, 0, sn + nn - 1) {
      int w = k < sn ? some[d][k] : none[d][k - sn], c = 0;
      FOR (j, 0, sn - 1) c += g[w][some[d][j]];
      if (c > best) best = c, u = w;
    }
    FOR (i, 0, sn - 1) {
      int v = some[d][i];
      if (g[u][v]) continue;
      int tsn = 0, tnn = 0;
      copy_n(all[d], an, all[d + 1]);
      all[d + 1][an] = v;
      FOR (j, 0, sn - 1) if (g[v][some[d][j]])
        some[d + 1][tsn++] = some[d][j];
      FOR (j, 0, nn - 1) if (g[v][none[d][j]])
        none[d + 1][tnn++] = none[d][j];
      dfs(d + 1, an + 1, tsn, tnn);
      some[d][i] = 0, none[d][nn++] = v;
    }
  }
  int solve() {
    iota(some[0], some[0] + n, 1);
    cnt = 0, dfs(0, 0, n, 0);
    return cnt;
  }
};
