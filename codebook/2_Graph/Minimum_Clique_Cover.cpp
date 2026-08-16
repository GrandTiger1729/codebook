struct Clique_Cover { // 0-base, O(n2^n)
  int co[1 << N], n, E[N];
  int dp[1 << N];
  void init(int _n) {
    n = _n, fill_n(dp, 1 << n, 0);
    fill_n(E, n, 0), fill_n(co, 1 << n, 0);
  }
  void add_edge(int u, int v) {
    E[u] |= 1 << v, E[v] |= 1 << u;
  }
  int solve() {
    FOR (i, 0, n - 1)
      co[1 << i] = E[i] | (1 << i);
    co[0] = (1 << n) - 1;
    dp[0] = (n & 1) * 2 - 1;
    FOR (i, 1, (1 << n) - 1) {
      int t = i & -i;
      dp[i] = -dp[i ^ t];
      co[i] = co[i ^ t] & co[t];
    }
    FOR (i, 0, (1 << n) - 1)
      co[i] = (co[i] & i) == i;
    fwt(co, 1 << n, 1);
    FOR (ans, 1, n - 1) {
      int sum = 0; // probabilistic
      FOR (i, 0, (1 << n) - 1)
        sum += (dp[i] *= co[i]);
      if (sum) return ans;
    }
    return n;
  }
};
