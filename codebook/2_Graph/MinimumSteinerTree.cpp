struct SteinerTree { // 0-base, need INF
  int n; // W <= 1e9 and V <= 100 already overflow int
  ll dst[N][N], dp[1 << T][N], tdst[N];
  ll vcst[N]; // the cost of vertexs
  void init(int _n) {
    n = _n;
    FOR (i, 0, n - 1) {
      fill_n(dst[i], n, INF);
      dst[i][i] = vcst[i] = 0;
    }
  }
  void chmin(ll &x, ll val) {
    x = min(x, val);
  }
  void add_edge(int ui, int vi, ll wi) {
    chmin(dst[ui][vi], wi);
  }
  void shortest_path() { // dst[i][j] ends up counting
    FOR (k, 0, n - 1) // every vcst on i -> j but i's
      FOR (i, 0, n - 1)
        FOR (j, 0, n - 1)
          chmin(dst[i][j], dst[i][k] + vcst[k] + dst[k][j]);
    FOR (i, 0, n - 1) FOR (j, 0, n - 1)
      if (i != j) dst[i][j] += vcst[j];
  }
  ll solve(const vector<int>& ter) {
    shortest_path();
    int t = (int)ter.size(), full = (1 << t) - 1;
    FOR (i, 0, full) fill_n(dp[i], n, INF);
    copy_n(vcst, n, dp[0]);
    FOR (msk, 1, full) {
      if (!(msk & (msk - 1))) {
        int who = __lg(msk);
        FOR (i, 0, n - 1)
          dp[msk][i] = vcst[ter[who]] + dst[ter[who]][i];
      }
      FOR (i, 0, n - 1)
        for (int sub = (msk - 1) & msk; sub; sub = (sub - 1) & msk)
          chmin(dp[msk][i], dp[sub][i] + dp[msk ^ sub][i] - vcst[i]);
      FOR (i, 0, n - 1) {
        tdst[i] = INF;
        FOR (j, 0, n - 1)
          chmin(tdst[i], dp[msk][j] + dst[j][i]);
      }
      copy_n(tdst, n, dp[msk]);
    }
    return *min_element(dp[full], dp[full] + n);
  }
}; // O(V 3^T + V^2 2^T)
