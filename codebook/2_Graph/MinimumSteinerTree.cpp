struct SteinerTree { // 0-base, need INF
  int n, mid[N][N], eid[N][N], fr[1 << T][N];
  ll dst[N][N], dp[1 << T][N], vcst[N]; // vertex cost
  vector<int> ans; // edge ids, set by solve()
  void init(int _n) {
    n = _n;
    FOR (i, 0, n - 1) {
      fill_n(dst[i], n, INF), fill_n(mid[i], n, -1);
      dst[i][i] = vcst[i] = 0;
    }
  }
  void add_edge(int u, int v, ll w, int id = -1) {
    if (w < dst[u][v]) dst[u][v] = w, eid[u][v] = id;
  }
  void shortest_path() { // dst[i][j] ends up counting
    FOR (k, 0, n - 1) // every vcst on i -> j but i's
      FOR (i, 0, n - 1)
        FOR (j, 0, n - 1)
          if (dst[i][k] + vcst[k] + dst[k][j] < dst[i][j])
            dst[i][j] = dst[i][k] + vcst[k] + dst[k][j],
            mid[i][j] = k;
    FOR (i, 0, n - 1) FOR (j, 0, n - 1)
      if (i != j) dst[i][j] += vcst[j];
  }
  void path(int i, int j) { // edges along i -> j
    if (i == j) return;
    int k = mid[i][j];
    if (k < 0) { ans.pb(eid[i][j]); return; }
    path(i, k), path(k, j);
  }
  void walk(int msk, int i) { // fr: 0 base case,
    int f = fr[msk][i]; // s > 0 split, -j - 1 from j
    if (!f) return;
    if (f > 0) walk(f, i), walk(msk ^ f, i);
    else path(-f - 1, i), walk(msk, -f - 1);
  }
  ll solve(const vector<int>& ter) {
    shortest_path();
    int t = (int)ter.size(), full = (1 << t) - 1;
    FOR (i, 0, full)
      fill_n(dp[i], n, INF), fill_n(fr[i], n, 0);
    copy_n(vcst, n, dp[0]);
    FOR (msk, 1, full) {
      if (!(msk & (msk - 1))) {
        int w = ter[__lg(msk)];
        dp[msk][w] = vcst[w];
      }
      FOR (i, 0, n - 1)
        for (int s = (msk - 1) & msk; s; s = (s - 1) & msk)
          if (dp[s][i] + dp[msk ^ s][i] - vcst[i] < dp[msk][i])
            dp[msk][i] = dp[s][i] + dp[msk ^ s][i] - vcst[i],
            fr[msk][i] = s;
      FOR (i, 0, n - 1) FOR (j, 0, n - 1) // in place is
        if (dp[msk][j] + dst[j][i] < dp[msk][i]) // fine
          dp[msk][i] = dp[msk][j] + dst[j][i],
          fr[msk][i] = -j - 1;
    }
    int b = min_element(dp[full], dp[full] + n) - dp[full];
    ans.clear(), walk(full, b);
    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end()), ans.end());
    return dp[full][b];
  }
}; // O(V 3^T + V^2 2^T)
