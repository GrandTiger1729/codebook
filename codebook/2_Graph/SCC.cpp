struct SCC { // need adj
  int n, dfcnt = 0, sccnt = 0;
  vector<int> dfn, low, scc, st;
  vector<vector<int>> scc_adj;
  SCC() {}
  SCC(int n) : n(n), dfn(n + 1), low(dfn), scc(dfn),
    scc_adj(n + 1) {}
  void tarjan(int u) { // scc[]: reverse topological order
    dfn[u] = low[u] = ++dfcnt;
    st.pb(u);
    for (int v : adj[u]) if (!scc[v]) { // new or on stack
      if (dfn[v]) low[u] = min(low[u], dfn[v]);
      else tarjan(v), low[u] = min(low[u], low[v]);
    }
    if (low[u] == dfn[u]) {
      sccnt++;
      while (1) {
        int x = st.back();
        st.pop_back(), scc[x] = sccnt;
        if (x == u) break;
      }
    }
  }
  void work() {
    FOR (i, 1, n) if (!dfn[i]) tarjan(i);
  }
  void build_adj() {
    FOR (i, 1, n) for (int j : adj[i])
      if (scc[i] != scc[j]) scc_adj[scc[i]].pb(scc[j]);
  }
};
