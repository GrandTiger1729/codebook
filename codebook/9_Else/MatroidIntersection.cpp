// max common independent set of two matroids on
// {0..n-1}. Each matroid provides
//   void build(const vector<int> &s) // s[i]: i in S?
//   bool ind(int x, int y) // S - x + y independent?
//                          // x = -1 means just S + y
// O(r) rounds, each O(n^2) oracle calls. Weighted ver:
// replace the BFS with Bellman-Ford minimising
// (sum of w(x), x in S / -w(y), y not in S; #edges).
template <class M1, class M2>
vector<int> matroid_isect(int n, M1 &m1, M2 &m2) {
  vector<int> sel(n, 0);
  while (1) {
    m1.build(sel), m2.build(sel);
    vector<int> pre(n, -2), q;
    FOR (y, 0, n - 1)
      if (!sel[y] && m1.ind(-1, y))
        pre[y] = -1, q.pb(y);
    int t = -1;
    for (int h = 0; h < (int)q.size(); h++) {
      int u = q[h];
      if (!sel[u] && m2.ind(-1, u)) {
        t = u;
        break;
      }
      FOR (v, 0, n - 1)
        if (pre[v] == -2)
          if (sel[u] ? !sel[v] && m1.ind(u, v)
                     : sel[v] && m2.ind(v, u))
            pre[v] = u, q.pb(v);
    }
    if (t < 0) return sel;
    for (int v = t; v != -1; v = pre[v]) sel[v] ^= 1;
  }
}
struct ColorMat { // <= cap[c] chosen of colour c
  vector<int> col, cap, cnt;
  ColorMat(vector<int> col, vector<int> cap)
    : col(col), cap(cap) {}
  void build(const vector<int> &s) {
    cnt.assign(cap.size(), 0);
    FOR (i, 0, (int)s.size() - 1)
      if (s[i]) cnt[col[i]]++;
  }
  bool ind(int x, int y) {
    int c = cnt[col[y]];
    if (x >= 0 && col[x] == col[y]) c--;
    return c < cap[col[y]];
  }
};
struct GraphMat { // chosen edges form a forest
  int n;
  vector<pair<int, int>> e;
  vector<int> sel, f;
  GraphMat(int n, vector<pair<int, int>> e)
    : n(n), e(e), f(n) {}
  void build(const vector<int> &s) { sel = s; }
  int fd(int a) {
    while (f[a] != a) a = f[a] = f[f[a]];
    return a;
  }
  bool ind(int x, int y) {
    iota(f.begin(), f.end(), 0);
    FOR (i, 0, (int)e.size() - 1)
      if (sel[i] && i != x) f[fd(e[i].F)] = fd(e[i].S);
    return fd(e[y].F) != fd(e[y].S);
  }
};
