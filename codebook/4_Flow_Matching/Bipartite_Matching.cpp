struct Bipartite_Matching { // 0-base
  int mp[N], mq[N], dis[N + 1], cur[N], l, r;
  vector<int> G[N + 1];
  bool dfs(int u) {
    for (int &i = cur[u]; i < (int)G[u].size(); i++) {
      int e = G[u][i], v = mq[e];
      if (v == l || (dis[v] == dis[u] + 1 && dfs(v)))
        return mp[mq[e] = u] = e, 1;
    }
    return dis[u] = -1, 0;
  }
  bool bfs() {
    queue<int> q;
    fill_n(dis, l + 1, -1);
    FOR (i, 0, l - 1) if (!~mp[i]) q.push(i), dis[i] = 0;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int e : G[u]) if (!~dis[mq[e]])
        q.push(mq[e]), dis[mq[e]] = dis[u] + 1;
    }
    return dis[l] != -1;
  }
  int matching() {
    int res = 0;
    fill_n(mp, l, -1), fill_n(mq, r, l);
    while (bfs()) {
      fill_n(cur, l, 0);
      FOR (i, 0, l - 1) res += (!~mp[i] && dfs(i));
    }
    return res; // (i, mp[i] != -1)
  }
  vector<int> cover() { // Konig, call after matching()
    vector<int> res; // L: i, R: l + e; dis from last bfs()
    FOR (i, 0, l - 1) if (!~dis[i]) res.pb(i);
    FOR (e, 0, r - 1) if (~dis[mq[e]]) res.pb(l + e);
    return res; // complement = max independent set
  }
  void add_edge(int s, int t) { G[s].pb(t); }
  void init(int _l, int _r) {
    l = _l, r = _r;
    FOR (i, 0, l) G[i].clear();
  }
};
