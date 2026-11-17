// g[u] = {(v, edge id)}, ids in [0, m); directed: only
// in g[u], undirected: in both. Walk from s using every
// edge once: (vertices, edge ids), or {} if impossible
// from s. Check degrees first (Theorem: Euler)
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
