// vertex order: s first, t last, others have neighbours
// on both sides; exists iff G + (s, t) is biconnected
vector<int> st_numbering(vector<vector<int>> g,
  int s, int t) {
  int n = g.size(), tk = 0;
  vector<int> ord, pre(n), low(n), pa(n, -1), sgn(n);
  g[s].insert(g[s].begin(), t); // t: first child of s
  auto dfs = [&](auto self, int u) -> void {
    ord.pb(u), low[u] = pre[u] = ++tk;
    for (int v : g[u])
      if (!pre[v]) {
        pa[v] = u, self(self, v);
        low[u] = min(low[u], low[v]);
      } else low[u] = min(low[u], pre[v]);
  };
  dfs(dfs, s);
  vector<int> nxt(n + 1, n), prv(n + 1, n), res;
  nxt[s] = t, prv[t] = s, sgn[s] = -1;
  for (int v : ord) if (v != s && v != t) {
    int p = pa[v], l = ord[low[v] - 1];
    if (sgn[l] > 0) prv[v] = p, nxt[v] = nxt[p];
    else nxt[v] = p, prv[v] = prv[p];
    nxt[prv[v]] = prv[nxt[v]] = v, sgn[p] = -sgn[l];
  }
  for (int v = s; v != n; v = nxt[v]) res.pb(v);
  return res;
}
