struct Bipartite_Matching { // 0-base
  int mp[N], mq[N], dis[N + 1], cur[N], l, r;
  vector<int> G[N + 1];
  bool dfs(int u) {
    for (int &i = cur[u]; i < (int)G[u].size(); i++) {
      int e = G[u][i];
      if (mq[e] == l || (dis[mq[e]] == dis[u] + 1 && dfs(mq[e])))
        return mp[mq[e] = u] = e, 1;
    }
    return dis[u] = -1, 0;
  }
  bool bfs() {
    queue<int> q;
    fill_n(dis, l + 1, -1);
    FOR (i, 0, l - 1)
      if (!~mp[i])
        q.push(i), dis[i] = 0;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int e : G[u])
        if (!~dis[mq[e]])
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
    vector<int> vl(l), vr(r), q, res; // L: i, R: l + e
    FOR (i, 0, l - 1) if (!~mp[i]) vl[i] = 1, q.pb(i);
    for (int h = 0; h < (int)q.size(); h++)
      for (int e : G[q[h]]) if (!vr[e]) {
        vr[e] = 1;
        if (mq[e] != l && !vl[mq[e]])
          vl[mq[e]] = 1, q.pb(mq[e]);
      }
    FOR (i, 0, l - 1) if (!vl[i]) res.pb(i);
    FOR (e, 0, r - 1) if (vr[e]) res.pb(l + e);
    return res; // complement = max independent set
  }
  void add_edge(int s, int t) { G[s].pb(t); }
  void init(int _l, int _r) {
    l = _l, r = _r;
    FOR (i, 0, l) G[i].clear();
  }
};
