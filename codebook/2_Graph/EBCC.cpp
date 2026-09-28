struct EBCC { // 1-base, need adj; bcc[v] = component id
  int n, dfcnt = 0, bccnt = 0;
  vector<int> dfn, low, bcc, st;
  EBCC() {}
  EBCC(int n) : n(n), dfn(n + 1), low(dfn), bcc(dfn) {}
  void tarjan(int u, int fa) {
    dfn[u] = low[u] = ++dfcnt;
    st.pb(u);
    bool skipped = 0; // skip one copy of the edge to fa
    for (int v : adj[u]) {
      if (v == fa && !skipped) skipped = 1;
      else if (dfn[v]) low[u] = min(low[u], dfn[v]);
      else tarjan(v, u), low[u] = min(low[u], low[v]);
    }
    if (low[u] == dfn[u]) { // fa-u is a bridge
      bccnt++;
      int x;
      do {
        x = st.back(), st.pop_back(), bcc[x] = bccnt;
      } while (x != u);
    }
  }
  void work() { FOR (i, 1, n) if (!dfn[i]) tarjan(i, 0); }
};
