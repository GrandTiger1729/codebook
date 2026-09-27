#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
// externals: N, in_cc (8_Geometry/point_in_circle.cpp), seg_strict_intersect (defined nowhere in the book)
const int N = 1000;
#include "8_Geometry/point_in_circle.cpp"
bool seg_strict_intersect(pdd p1, pdd p2, pdd p3, pdd p4) { // interiors cross at a single point
  return ori(p1, p2, p3) * ori(p1, p2, p4) < 0 && ori(p3, p4, p1) * ori(p3, p4, p2) < 0;
}
#include "8_Geometry/DelaunayTriangulation_dq.cpp"
// Check: planar straight-line graph, #edges = 3n-3-h (h = points on hull boundary) so it
// is a full triangulation (n-1 if collinear), every triangular face has an empty
// circumcircle, and #faces = 2n-2-h.
typedef __int128 L;
L cr(pll o, pll a, pll b) { return (L)(a.X - o.X) * (b.Y - o.Y) - (L)(a.Y - o.Y) * (b.X - o.X); }
int sg(L x) { return (x > 0) - (x < 0); }
bool onSeg(pll a, pll b, pll p) { // p strictly inside segment ab
  return cr(a, b, p) == 0 && (L)(p.X - a.X) * (p.X - b.X) + (L)(p.Y - a.Y) * (p.Y - b.Y) < 0;
}
bool properCross(pll a, pll b, pll c, pll d) {
  return sg(cr(a, b, c)) * sg(cr(a, b, d)) < 0 && sg(cr(c, d, a)) * sg(cr(c, d, b)) < 0;
}
L incirc(pll a, pll b, pll c, pll d) { // >0 iff d strictly inside circumcircle of ccw abc
  L m[3][3]; pll q[3] = {a, b, c};
  FOR (i, 0, 2) { ll x = q[i].X - d.X, y = q[i].Y - d.Y; m[i][0] = x, m[i][1] = y, m[i][2] = (L)x * x + (L)y * y; }
  return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}
int hullBoundary(vector<pll> p) { // #points on convex hull boundary (incl. collinear)
  sort(ALL(p));
  int n = SZ(p);
  if (n <= 2) return n;
  vector<pll> h(2 * n); int k = 0;
  FOR (s, 0, 1) {
    int st = k;
    for (auto q : p) { while (k >= st + 2 && cr(h[k - 2], h[k - 1], q) < 0) k--; h[k++] = q; }
    k--; reverse(ALL(p));
  }
  set<pll> b(h.begin(), h.begin() + k);
  if (k < 3 || all_of(ALL(p), [&](pll q) { return cr(p[0], p.back(), q) == 0; })) return -1; // collinear
  return SZ(b);
}
pll arr[N];
int main() {
  int cnt = 0;
  FOR (it, 1, 2500) {
    int mode = it % 4, L0 = mode == 0 ? 3 : mode == 1 ? 6 : mode == 2 ? 30 : 1000;
    int n = rnd(1, mode == 3 ? 120 : 40);
    set<pll> s;
    if (it % 50 == 0) { FOR (i, 0, n - 1) s.insert(pll(i, 2 * i)); } // collinear
    else FOR (i, 0, n - 1) s.insert(pll(rnd(0, L0), rnd(0, L0)));
    vector<pll> p(ALL(s));
    shuffle(ALL(p), rng);
    n = SZ(p);
    FOR (i, 0, n - 1) arr[i] = p[i];
    tool.init(n, arr);
    // gather edges in original indices
    set<pii> E;
    FOR (i, 0, n - 1) for (auto e : tool.head[i]) {
      int u = tool.oidx[i], v = tool.oidx[e.id];
      assert(u != v);
      assert(!E.count({u, v}));
      E.insert({u, v});
    }
    for (auto [u, v] : E) assert(E.count({v, u}));
    vector<pii> ed;
    for (auto [u, v] : E) if (u < v) ed.pb(u, v);
    // planar straight-line: no proper crossing, no point in the middle of an edge
    for (auto [u, v] : ed) FOR (w, 0, n - 1) assert(!onSeg(p[u], p[v], p[w]));
    FOR (i, 0, SZ(ed) - 1) FOR (j, i + 1, SZ(ed) - 1)
      assert(!properCross(p[ed[i].F], p[ed[i].S], p[ed[j].F], p[ed[j].S]));
    int h = hullBoundary(p);
    if (h < 0 || n <= 2) { assert(SZ(ed) == n - 1); cnt++; continue; }
    assert(SZ(ed) == 3 * n - 3 - h);
    // faces = empty 3-cycles; each must have an empty circumcircle
    vector<set<int>> adj(n);
    for (auto [u, v] : ed) adj[u].insert(v), adj[v].insert(u);
    int faces = 0;
    for (auto [u, v] : ed) for (int w : adj[u]) if (w > v && adj[v].count(w)) {
      int a = u, b = v, c = w;
      if (cr(p[a], p[b], p[c]) == 0) continue;
      if (cr(p[a], p[b], p[c]) < 0) swap(b, c);
      bool empty = true;
      FOR (x, 0, n - 1) if (x != a && x != b && x != c)
        if (cr(p[a], p[b], p[x]) >= 0 && cr(p[b], p[c], p[x]) >= 0 && cr(p[c], p[a], p[x]) >= 0) empty = false;
      if (!empty) continue;
      faces++;
      FOR (x, 0, n - 1) assert(incirc(p[a], p[b], p[c], p[x]) <= 0);
    }
    assert(faces == 2 * n - 2 - h);
    cnt++;
  }
  printf("Delaunay_dq: %d point sets OK (planar, 3n-3-h edges, 2n-2-h empty-circumcircle faces)\n", cnt);
}
