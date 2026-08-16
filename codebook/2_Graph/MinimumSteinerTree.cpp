struct SteinerTree { // 0-base
  int n, dst[N][N], dp[1 << T][N], tdst[N];
  int vcst[N]; // the cost of vertexs
  void init(int _n) {
    n = _n;
    FOR (i, 0, n - 1) {
      fill_n(dst[i], n, INF);
      dst[i][i] = vcst[i] = 0;
    }
  }
  void chmin(int &x, int val) {
    x = min(x, val);
  }
  void add_edge(int ui, int vi, int wi) {
    chmin(dst[ui][vi], wi);
  }
  void shortest_path() {
    FOR (k, 0, n - 1) FOR (i, 0, n - 1) FOR (j, 0, n - 1)
      chmin(dst[i][j], dst[i][k] + dst[k][j]);
  }
  int solve(const vector<int>& ter) {
    shortest_path();
    int t = SZ(ter), full = (1 << t) - 1;
    FOR (i, 0, full) fill_n(dp[i], n, INF);
    copy_n(vcst, n, dp[0]);
    for (int msk = 1; msk <= full; ++msk) {
      if (!(msk & (msk - 1))) {
        int who = __lg(msk);
        for (int i = 0; i < n; ++i)
          dp[msk][i] = vcst[ter[who]] + dst[ter[who]][i];
      }
      FOR (i, 0, n - 1)
        for (int sub = (msk - 1) & msk; sub; sub = (sub - 1) & msk)
          chmin(dp[msk][i], dp[sub][i] + dp[msk ^ sub][i] - vcst[i]);
      FOR (i, 0, n - 1) {
        tdst[i] = INF;
        FOR (j, 0, n - 1) chmin(tdst[i], dp[msk][j] + dst[j][i]);
      }
      copy_n(tdst, n, dp[msk]);
    }
    return *min_element(dp[full], dp[full] + n);
  }
}; // O(V 3^T + V^2 2^T)
