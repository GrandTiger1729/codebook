struct VBCC { // 1-base, need adj; bcc[v] = comps of v
  int n, dfcnt = 0, bccnt = 0;
  vector<int> dfn, low, st;
  vector<vector<int>> bcc;
  VBCC() {}
  VBCC(int n) : n(n), dfn(n + 1), low(dfn), bcc(n + 1) {}
  void tarjan(int u, int fa) { // root: fa = 0
    dfn[u] = low[u] = ++dfcnt;
    st.pb(u);
    for (int v : adj[u]) {
      if (v == fa) continue;
      if (dfn[v]) low[u] = min(low[u], dfn[v]);
      else {
        tarjan(v, u), low[u] = min(low[u], low[v]);
        if (low[v] < dfn[u]) continue;
        bccnt++; // u cuts v's subtree off
        while (1) {
          int x = st.back();
          st.pop_back(), bcc[x].pb(bccnt);
          if (x == v) break;
        }
        bcc[u].pb(bccnt);
      }
    }
    if (!fa) { // u is a root
      st.pop_back();
      if (bcc[u].empty()) bcc[u].pb(++bccnt); // isolated
    }
  }
  void work() { FOR (i, 1, n) if (!dfn[i]) tarjan(i, 0); }
};
