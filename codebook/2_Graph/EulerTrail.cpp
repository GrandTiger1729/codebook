// g[u]: (v, edge id), undirected: both ways. Trail from
// s using all m edges, {} if none; check degrees first
pair<vector<int>, vector<int>> euler_trail(int s,
  const vector<vector<pair<int, int>>> &g, int m) {
  vector<int> it(g.size()), used(m), vs, es;
  vector<pair<int, int>> st{{s, -1}}; // (vertex, edge)
  while (!st.empty()) {
    auto [u, e] = st.back();
    auto &i = it[u];
    while (i < (int)g[u].size() && used[g[u][i].S]) i++;
    if (i == (int)g[u].size()) {
      vs.pb(u), es.pb(e), st.pop_back();
      continue;
    }
    auto [v, id] = g[u][i++];
    used[id] = 1, st.pb(v, id);
  }
  es.pop_back();
  if ((int)es.size() != m) return {};
  reverse(vs.begin(), vs.end());
  reverse(es.begin(), es.end());
  return {vs, es};
}
