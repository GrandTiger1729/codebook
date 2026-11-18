// g[v]: children of v, rooted at 0. Define first:
//   Path vertex(int v); Path add_vertex(Point, int v);
//   Path compress(Path top, Path bottom);
//   Point add_edge(Path); Point rake(Point, Point);
// After v's data changes: update(v). Answer: path[root]
struct StaticTopTree {
  using pii = pair<int, int>; // (node, size)
  enum Type
  { Vertex, Compress, Rake, AddEdge, AddVertex };
  vector<vector<int>> g;
  vector<int> P, L, R;
  vector<Type> T;
  vector<Path> path;
  vector<Point> point;
  int root;
  StaticTopTree(vector<vector<int>> _g) : g(_g) {
    int n = g.size();
    vector<int> sz(n, 1), ord{0};
    FOR (i, 0, n - 1) for (int v : g[ord[i]]) ord.pb(v);
    for (int i = n - 1; i >= 0; i--) {
      auto &c = g[ord[i]]; // heavy child first
      for (int v : c) sz[ord[i]] += sz[v];
      FOR (j, 1, (int)c.size() - 1)
        if (sz[c[j]] > sz[c[0]]) swap(c[0], c[j]);
    }
    P = L = R = vector<int>(n, -1), T.resize(n);
    root = heavy(0).F;
    path.resize(P.size()), point.resize(P.size());
    build(root);
  }
  int add(int k, int l, int r, Type t) {
    if (k < 0)
      k = P.size(), P.pb(-1), L.pb(), R.pb(), T.pb();
    L[k] = l, R[k] = r, T[k] = t;
    if (l >= 0) P[l] = k;
    if (r >= 0) P[r] = k;
    return k;
  }
  pii merge(const vector<pii> &a, Type t) {
    if (a.size() == 1) return a[0];
    int u = 0;
    for (auto [i, s] : a) u += s;
    vector<pii> b, c; // split by size, keep order
    for (auto [i, s] : a)
      (u > s ? b : c).pb(i, s), u -= 2 * s;
    auto [i, si] = merge(b, t);
    auto [j, sj] = merge(c, t);
    return {add(-1, i, j, t), si + sj};
  }
  pii heavy(int v) { // the heavy path from v down
    vector<pii> a{vert(v)};
    while (!g[v].empty()) a.pb(vert(v = g[v][0]));
    return merge(a, Compress);
  }
  pii vert(int v) { // v and all its light subtrees
    vector<pii> a;
    FOR (j, 1, (int)g[v].size() - 1) {
      auto [k, s] = heavy(g[v][j]);
      a.pb(add(-1, k, -1, AddEdge), s);
    }
    auto [k, s] = a.empty() ? pii(-1, 0) : merge(a, Rake);
    Type t = k < 0 ? Vertex : AddVertex;
    return {add(v, k, -1, t), s + 1};
  }
  void calc(int k) {
    if (T[k] == Vertex) path[k] = vertex(k);
    else if (T[k] == AddVertex)
      path[k] = add_vertex(point[L[k]], k);
    else if (T[k] == AddEdge)
      point[k] = add_edge(path[L[k]]);
    else if (T[k] == Rake)
      point[k] = rake(point[L[k]], point[R[k]]);
    else path[k] = compress(path[L[k]], path[R[k]]);
  }
  void build(int k) {
    if (L[k] >= 0) build(L[k]);
    if (R[k] >= 0) build(R[k]);
    calc(k);
  }
  void update(int v) { for (; v >= 0; v = P[v]) calc(v); }
};
