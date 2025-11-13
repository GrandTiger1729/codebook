struct VBCC {  // need adj
  int n, dfcnt = 0, bccnt = 0;
  vector<int> dfn, low, st;
  vector<vector<int>> bcc;
  VBCC() {}
  VBCC(int n) : n(n) {
    dfn = low = vector<int>(n + 1, 0);
    bcc = vector<vector<int>>(n + 1, vector<int>());
  }
  void tarjan(int pos, int fa) {
    dfn[pos] = low[pos] = ++dfcnt;
    st.pb(pos);
    for (int np : adj[pos]) {
      if (np != fa) {
        if (dfn[np] == 0) {
          tarjan(np, pos);
          low[pos] = min(low[pos], low[np]);
          if (dfn[pos] <= low[np]) {
            bccnt++;
            while (1) {
              int x = st.back();
              bcc[x].pb(bccnt);
              st.pop_back();
              if (x == np) break;
            }
            bcc[pos].pb(bccnt);
          }
        } else { low[pos] = min(low[pos], dfn[np]); }
      }
    }
    if (pos == fa) {
      st.pop_back();
      if (bcc[pos].size() == 0) bcc[pos].pb(++bccnt);
    }
  }
  void work() {
    FOR (i, 1, n) if (dfn[i] == 0) tarjan(i, i);
  }
};
