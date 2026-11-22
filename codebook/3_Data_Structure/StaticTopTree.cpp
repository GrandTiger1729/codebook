// Static Top Tree (O(log N), no online link/cut)
// Weighted edges: subdivide each edge as a vertex.
// g[v]: children of v, root 0. Define before it:
// Path vertex(v), Path add_vertex(Point light, v),
// Point add_edge(Path), Point rake(Point, Point),
// Path compress(Path top, Path bottom).
// Global: path[root]. Update v: update(v).
// Subtree(v): climb Compress, merge R[p] if L[p].
// Reroot(v): keep rpath; walk root->v (up/down/rk).
// Reroot sub(v,r): if r in sub(v), complement of
//   child c toward r rooted at v; else sub(v).
// Path(u,v): merge Compress segs (rpath if dir).
// Lazy sub: push root->v, tag v & right sibs.
// Lazy path: split path-only vs cluster lazy tag.
// Bsearch: walk from root (dyn centroid, etc.).
// Apps: diameter, nearest/dist sum, DDP.
// Batch k: O(k log(N/k)). Persist: copy O(log N).
struct StaticTopTree {
  using pii = pair<int, int>; // (node, size)
  enum Type
  { Vertex, Compress, Rake, AddEdge, AddVertex };
  vector<vector<int>> g;
  vector<int> P, L, R;
  vector<Type> T;
  vector<Path> path, rpath;
  vector<Point> point;
  int root;
  StaticTopTree(vector<vector<int>> _g) : g(_g) {
    int n = g.size();
    vector<int> sz(n, 1), ord{0};
    FOR (i, 0, n - 1) for (int v : g[ord[i]]) ord.pb(v);
    for (int i = n - 1; i >= 0; i--) {
      auto &c = g[ord[i]];
      for (int v : c) sz[ord[i]] += sz[v];
      FOR (j, 1, (int)c.size() - 1)
        if (sz[c[j]] > sz[c[0]]) swap(c[0], c[j]);
    }
    P = L = R = vector<int>(n, -1), T.resize(n);
    root = heavy(0).F;
    path.resize(P.size()), rpath.resize(P.size());
    point.resize(P.size());
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
    vector<pii> b, c;
    for (auto [i, s] : a)
      (u > s ? b : c).pb(i, s), u -= 2 * s;
    auto [i, si] = merge(b, t);
    auto [j, sj] = merge(c, t);
    return {add(-1, i, j, t), si + sj};
  }
  pii heavy(int v) {
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
    if (T[k] == Vertex) path[k] = rpath[k] = vertex(k);
    else if (T[k] == AddVertex)
      path[k] = rpath[k] = add_vertex(point[L[k]], k);
    else if (T[k] == AddEdge)
      point[k] = add_edge(path[L[k]]);
    else if (T[k] == Rake)
      point[k] = rake(point[L[k]], point[R[k]]);
    else {
      path[k] = compress(path[L[k]], path[R[k]]);
      rpath[k] = compress(rpath[R[k]], rpath[L[k]]);
    }
  }
  void build(int k) {
    if (L[k] >= 0) build(L[k]);
    if (R[k] >= 0) build(R[k]);
    calc(k);
  }
  void update(int v) { for (; v >= 0; v = P[v]) calc(v); }
  Path query_subtree(int v) {
    Path res = path[v];
    for (int u = v, p = P[u]; p >= 0 && T[p] == Compress;
      u = p, p = P[u])
      if (u == L[p]) res = compress(res, path[R[p]]);
    return res;
  }
  Path query_reroot(int v) {
    vector<int> stk;
    for (int u = v; u >= 0; u = P[u]) stk.pb(u);
    Path up, down; Point rk;
    bool hu = 0, hd = 0, hr = 0; int w = -1;
    auto add_rk = [&](Point x) {
      rk = hr ? rake(rk, x) : x, hr = 1;
    };
    for (int i = (int) stk.size() - 1; i > 0; i--) {
      int p = stk[i], u = stk[i - 1];
      if (T[p] == Compress) {
        if (u == L[p]) {
          Path x = path[R[p]];
          down = hd ? compress(x, down) : x, hd = 1;
        } else {
          Path x = rpath[L[p]];
          up = hu ? compress(x, up) : x, hu = 1;
        }
      } else if (T[p] == AddVertex) {
        w = p, hr = 0;
        if (hu) add_rk(add_edge(up));
        if (hd) add_rk(add_edge(down));
      } else if (T[p] == Rake) {
        add_rk(point[u == L[p] ? R[p] : L[p]]);
      } else { // AddEdge
        up = hr ? add_vertex(rk, w) : vertex(w);
        hu = 1, hd = 0;
      }
    }
    hr = 0;
    if (T[v] == AddVertex) add_rk(point[L[v]]);
    if (hu) add_rk(add_edge(up));
    if (hd) add_rk(add_edge(down));
    return hr ? add_vertex(rk, v) : vertex(v);
  }
};
