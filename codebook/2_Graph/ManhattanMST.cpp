// returns O(n) edges {w, i, j} containing a Manhattan
// MST of the points; run Kruskal on them
vector<array<ll, 3>> manhattan_mst(
  vector<pair<ll, ll>> p) {
  vector<int> id(p.size());
  iota(id.begin(), id.end(), 0);
  vector<array<ll, 3>> edges;
  FOR (k, 0, 3) {
    sort(id.begin(), id.end(), [&](int i, int j) {
      return p[i].F - p[j].F < p[j].S - p[i].S;
    });
    map<ll, int> sweep; // -y -> index
    for (int i : id) {
      auto it = sweep.lower_bound(-p[i].S);
      for (; it != sweep.end(); it = sweep.erase(it)) {
        int j = it->S;
        ll dx = p[i].F - p[j].F, dy = p[i].S - p[j].S;
        if (dy > dx) break;
        edges.pb(array<ll, 3>{dx + dy, i, j});
      }
      sweep[-p[i].S] = i;
    }
    for (auto &[x, y] : p)
      if (k & 1) x = -x;
      else swap(x, y);
  }
  return edges;
}
