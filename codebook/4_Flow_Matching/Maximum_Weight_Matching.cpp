// 1-based; add_edge(u, v, w) needs w > 0 (0 = no edge)
// solve() once: {weight, size}; match[u]: partner, 0 if none
struct WeightGraph {
  struct edge { int u, v, w; }; int n, nx, wm = 0;
  vector<int> lab; vector<vector<edge>> g;
  vector<int> slk, match, st, pa, sl, vis;
  vector<vector<int>> flo, flo_from; queue<int> q;
  WeightGraph(int n_) : n(n_), nx(n * 2), lab(nx + 1),
    g(nx + 1, vector<edge>(nx + 1)), slk(nx + 1),
    flo(nx + 1), flo_from(nx + 1, vector(n + 1, 0)) {
    match = st = pa = sl = vis = slk;
    FOR (u, 1, n) FOR (v, 1, n) g[u][v] = {u, v, 0};
    FOR (u, 1, n) st[u] = u, flo_from[u][u] = u;
  }
  void add_edge(int u, int v, int w)
  { g[u][v].w = g[v][u].w = w, wm = max(wm, w); }
  int E(edge e) { return lab[e.u] + lab[e.v] - e.w * 2; }
  void upd(int u, int x) {
    if (!slk[x] || E(g[u][x]) < E(g[slk[x]][x])) slk[x] = u;
  }
  void set_slk(int x) {
    slk[x] = 0;
    FOR (u, 1, n) if (g[u][x].w > 0 && st[u] != x &&
      sl[st[u]] == 0) upd(u, x);
  }
  void push(int x) {
    if (x > n) for (int y : flo[x]) push(y); else q.push(x);
  }
  void set_st(int x, int b) {
    st[x] = b; if (x > n) for (int y : flo[x]) set_st(y, b);
  }
  vector<int> split(vector<int> &f, int xr) {
    auto it = ranges::find(f, xr);
    if (int p = it - f.begin(); p % 2)
      reverse(f.begin() + 1, f.end()), it = f.end() - p;
    vector<int> res(f.begin(), it); f.erase(f.begin(), it);
    return res;
  }
  void set_match(int u, int v) {
    match[u] = g[u][v].v; if (u <= n) return;
    int xr = flo_from[u][g[u][v].u];
    auto &f = flo[u], z = split(f, xr);
    FOR (i, 0, (int)z.size() - 1) set_match(z[i], z[i ^ 1]);
    set_match(xr, v); f.insert(f.end(), z.begin(), z.end());
  }
  void augment(int u, int v) {
    for (int x = st[match[u]]; set_match(u, v), x;
      x = st[match[u]]) set_match(v = x, u = st[pa[x]]);
  }
  int lca(int u, int v) {
    static int t = 0;
    for (++t; u || v; swap(u, v)) if (u) {
      if (vis[u] == t) return u;
      vis[u] = t, u = st[match[u]];
      if (u) u = st[pa[u]];
    }
    return 0;
  }
  void add_blossom(int u, int o, int v) {
    int b = n + 1; while (st[b]) b++;
    lab[b] = sl[b] = 0, match[b] = match[o];
    auto &f = flo[b] = {o};
    for (int t : {u, v}) {
      reverse(f.begin() + 1, f.end());
      for (int x = t, y; x != o; x = st[pa[y]])
        f.pb(x), f.pb(y = st[match[x]]), push(y);
    }
    set_st(b, b);
    FOR (x, 1, nx) g[b][x].w = g[x][b].w = 0;
    ranges::fill(flo_from[b], 0);
    for (int xs : f) {
      FOR (x, 1, nx)
        if (g[b][x].w == 0 || E(g[xs][x]) < E(g[b][x]))
          g[b][x] = g[xs][x], g[x][b] = g[x][xs];
      FOR (x, 1, n) if (flo_from[xs][x]) flo_from[b][x] = xs;
    }
    set_slk(b);
  }
  void expand_blossom(int b) {
    for (int x : flo[b]) set_st(x, x);
    int xr = flo_from[b][g[b][pa[b]].u], xs = -1;
    for (int x : split(flo[b], xr)) {
      if (xs == -1) { xs = x; continue; }
      pa[xs] = g[x][xs].u, sl[xs] = 1, sl[x] = 0;
      slk[xs] = 0, set_slk(x), push(x), xs = -1;
    }
    for (int x : flo[b])
      if (x == xr) sl[x] = 1, pa[x] = pa[b];
      else sl[x] = -1, set_slk(x);
    st[b] = 0;
  }
  bool on_found_edge(const edge &e) {
    if (int u = st[e.u], v = st[e.v]; sl[v] == -1) {
      int nu = st[match[v]];
      pa[v] = e.u, sl[v] = 1;
      slk[v] = slk[nu] = sl[nu] = 0, push(nu);
    } else if (sl[v] == 0) {
      if (int o = lca(u, v)) add_blossom(u, o, v);
      else { augment(u, v), augment(v, u); return true; }
    }
    return false;
  }
  bool matching() {
    ranges::fill(sl, -1), ranges::fill(slk, 0), q = {};
    FOR (x, 1, nx) if (st[x] == x && !match[x])
      pa[x] = sl[x] = 0, push(x);
    if (q.empty()) return false;
    for (;;) {
      while (q.size()) {
        int u = q.front(); q.pop();
        if (sl[st[u]] == 1) continue;
        FOR (v, 1, n) if (g[u][v].w > 0 && st[u] != st[v]) {
          if (E(g[u][v])) upd(u, st[v]);
          else if (on_found_edge(g[u][v])) return true;
        }
      }
      int d = INT_MAX;
      FOR (u, 1, n) if (sl[st[u]] == 0) d = min(d, lab[u]);
      FOR (b, n + 1, nx) if (st[b] == b && sl[b] == 1)
        d = min(d, lab[b] / 2);
      FOR (x, 1, nx) if (st[x] == x && slk[x] && sl[x] <= 0)
        d = min(d, E(g[slk[x]][x]) / (sl[x] + 2));
      FOR (u, 1, n)
        if (sl[st[u]] == 1) lab[u] += d;
        else if (sl[st[u]] == 0 && (lab[u] -= d) <= 0)
          return false;
      FOR (b, n + 1, nx) if (st[b] == b && sl[b] >= 0)
        lab[b] += d * (2 - 4 * sl[b]);
      FOR (x, 1, nx) if (int s = slk[x]; st[x] == x && s &&
        st[s] != x && !E(g[s][x]) && on_found_edge(g[s][x]))
        return true;
      FOR (b, n + 1, nx)
        if (st[b] == b && sl[b] == 1 && lab[b] == 0)
          expand_blossom(b);
    }
  }
  pair<ll, int> solve() {
    ll tot = 0; int cnt = 0;
    ranges::fill(lab, wm);
    while (matching()) cnt++;
    FOR (u, 1, n) if (match[u] < u) tot += g[u][match[u]].w;
    return {tot, cnt};
  }
};
