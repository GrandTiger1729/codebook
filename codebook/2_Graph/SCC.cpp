struct SCC { // need adj
  int n, dfcnt = 0, sccnt = 0;
  vector<int> dfn, scc, st;
  vector<vector<int>> scc_adj;
  SCC() {}
  SCC(int n)
    : n(n), dfn(n + 1), scc(n + 1), scc_adj(n + 1) {}
  void tarjan(int u) { // scc[] numbered sink first
    int lw = dfn[u] = ++dfcnt, x;
    st.pb(u);
    for (int v : adj[u]) if (!scc[v]) {
      if (!dfn[v]) tarjan(v);
      lw = min(lw, dfn[v]);
    }
    if (lw == dfn[u]) {
      sccnt++;
      do {
        x = st.back(), st.pop_back(), scc[x] = sccnt;
      } while (x != u);
    }
    dfn[u] = lw; // dfn[] now holds the low-link
  }
  void work() {
    FOR (i, 1, n) if (!dfn[i]) tarjan(i);
  }
  void build_adj() {
    FOR (i, 1, n) for (int j : adj[i])
      if (scc[i] != scc[j])
        scc_adj[scc[i]].pb(scc[j]);
  }
};
