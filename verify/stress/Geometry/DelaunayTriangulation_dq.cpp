#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
// externals: N, in_cc (8_Geometry/point_in_circle.cpp), seg_strict_intersect (defined nowhere in the book)
const int N = 100016; // cocircular generator overshoots n by < 9
#include "8_Geometry/point_in_circle.cpp"
bool seg_strict_intersect(pdd p1, pdd p2, pdd p3, pdd p4) { // interiors cross at a single point
  return ori(p1, p2, p3) * ori(p1, p2, p4) < 0 && ori(p3, p4, p1) * ori(p3, p4, p2) < 0;
}
#include "8_Geometry/DelaunayTriangulation_dq.cpp"
// With the book's prelude ori / abs2 / in_cc's cross on pll go through pdd: exact only while
// 8 |coord|^2 < 2^53, i.e. |coord| <= ~3.3e7. The same template on the integer prelude
// (Default_code_int, not in the book) is all ll / __int128: tested at |coord| <= 1e9.
namespace ints {
#include "8_Geometry/Default_code_int.cpp"
#include "8_Geometry/point_in_circle.cpp"
bool seg_strict_intersect(pll p1, pll p2, pll p3, pll p4) {
  return ori(p1, p2, p3) * ori(p1, p2, p4) < 0 && ori(p3, p4, p1) * ori(p3, p4, p2) < 0;
}
#include "8_Geometry/DelaunayTriangulation_dq.cpp"
}
// Check: planar straight-line graph, #edges = 3n-3-h (h = points on hull boundary) so it
// is a full triangulation (n-1 if collinear), every triangular face has an empty
// circumcircle, and #faces = 2n-2-h. Big n: faces traced from the angle-sorted rotation
// system are 2n-2-h CCW triangles whose areas sum to the hull area plus the outer face (the
// h boundary points), and every inner edge is locally Delaunay. All exact (__int128).
typedef __int128 L;
L cr(pll o, pll a, pll b) { return (L)(a.X - o.X) * (b.Y - o.Y) - (L)(a.Y - o.Y) * (b.X - o.X); }
int sg(L x) { return (x > 0) - (x < 0); }
bool onSeg(pll a, pll b, pll p) { // p strictly inside segment ab
  return cr(a, b, p) == 0 && (L)(p.X - a.X) * (p.X - b.X) + (L)(p.Y - a.Y) * (p.Y - b.Y) < 0;
}
bool properCross(pll a, pll b, pll c, pll d) {
  return sg(cr(a, b, c)) * sg(cr(a, b, d)) < 0 && sg(cr(c, d, a)) * sg(cr(c, d, b)) < 0;
}
// sign of the lifted determinant over rows (p_i - d, |p_i - d|^2): > 0 iff d strictly
// inside the circumcircle of CCW abc. |z c| reaches 8e18 * 8e18 at |coord| = 1e9, so z is
// split into 32-bit halves (det = H * 2^32 + Lo).
int incirc(pll a, pll b, pll c, pll d) {
  pll q[3] = {a - d, b - d, c - d};
  L H = 0, Lo = 0;
  FOR (i, 0, 2) {
    pll u = q[(i + 1) % 3], v = q[(i + 2) % 3];
    L k = (L)u.X * v.Y - (L)u.Y * v.X, z = (L)q[i].X * q[i].X + (L)q[i].Y * q[i].Y;
    H += (z >> 32) * k, Lo += (z & 0xffffffff) * k;
  }
  L carry = Lo >= 0 ? Lo >> 32 : -((-Lo + 0xffffffff) >> 32); // floor(Lo / 2^32)
  H += carry, Lo -= carry << 32;
  return H ? sg(H) : Lo > 0;
}
vector<pll> hullOf(vector<pll> p, bool keepCollinear) { // CCW
  sort(ALL(p));
  int n = SZ(p);
  if (n <= 2) return p;
  vector<pll> h(2 * n); int k = 0;
  FOR (s, 0, 1) {
    int st = k;
    for (auto q : p) { while (k >= st + 2 && (keepCollinear ? cr(h[k - 2], h[k - 1], q) < 0 : cr(h[k - 2], h[k - 1], q) <= 0)) k--; h[k++] = q; }
    k--; reverse(ALL(p));
  }
  h.resize(k);
  return h;
}
int hullBoundary(vector<pll> p) { // #points on convex hull boundary (incl. collinear), -1: collinear
  if (SZ(p) <= 2) return SZ(p);
  if (all_of(ALL(p), [&](pll q) { return cr(p[0], p[1], q) == 0; })) return -1;
  auto h = hullOf(p, true);
  return SZ(set<pll>(ALL(h)));
}
L area2(const vector<pll> &v) {
  L a = 0;
  FOR (i, 0, SZ(v) - 1) a += (L)v[i].X * v[(i + 1) % SZ(v)].Y - (L)v[i].Y * v[(i + 1) % SZ(v)].X;
  return a;
}
// the brute of the original test: O(E^2 + F n)
void checkSmall(const vector<pll> &p, const vector<pii> &ed, int h) {
  int n = SZ(p);
  for (auto [u, v] : ed) FOR (w, 0, n - 1) assert(!onSeg(p[u], p[v], p[w]));
  FOR (i, 0, SZ(ed) - 1) FOR (j, i + 1, SZ(ed) - 1)
    assert(!properCross(p[ed[i].F], p[ed[i].S], p[ed[j].F], p[ed[j].S]));
  if (h < 0 || n <= 2) return;
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
}
int half(pll v) { return v.Y < 0 || (v.Y == 0 && v.X < 0); }
void checkBig(const vector<pll> &p, const vector<pii> &ed, int h) {
  int n = SZ(p);
  vector<vector<int>> adj(n);
  for (auto [u, v] : ed) adj[u].pb(v), adj[v].pb(u);
  FOR (u, 0, n - 1) {
    assert(SZ(adj[u]));
    sort(ALL(adj[u]), [&](int a, int b) {
      pll x = p[a] - p[u], y = p[b] - p[u];
      return half(x) != half(y) ? half(x) < half(y) : cr(pll(0, 0), x, y) > 0;
    });
  }
  auto idx = [&](int v, int u) { return int(find(ALL(adj[v]), u) - adj[v].begin()); }; // degree ~ 6
  auto nxt = [&](int u, int v) { // face left of u->v continues with v->w
    int d = SZ(adj[v]);
    return adj[v][(idx(v, u) + d - 1) % d];
  };
  vector<vector<char>> seen(n);
  FOR (u, 0, n - 1) seen[u].assign(SZ(adj[u]), 0);
  auto mark = [&](int x, int y) { char &c = seen[x][idx(x, y)]; return c ? false : (c = 1, true); };
  int inner = 0, outer = 0;
  L sum = 0;
  for (auto [a, b] : ed) for (auto [u, v] : {pii(a, b), pii(b, a)}) {
    if (seen[u][idx(u, v)]) continue;
    vector<pll> f;
    for (int x = u, y = v; mark(x, y);) {
      f.pb(p[x]);
      int z = nxt(x, y);
      x = y, y = z;
    }
    L A = area2(f);
    if (A > 0) {
      assert(SZ(f) == 3), inner++, sum += A;
      int w = nxt(u, v), x = nxt(v, u); // right face of u->v is (v, u, x)
      if (cr(p[v], p[u], p[x]) > 0) assert(incirc(p[u], p[v], p[w], p[x]) <= 0);
    } else outer++, assert(SZ(f) == h);
  }
  assert(outer == 1 && inner == 2 * n - 2 - h && sum == area2(hullOf(p, false)));
}
template <class D> int run(D &tool, ll M, int it, vector<pll> &arr) {
  int mode = it % 4;
  ll L0 = mode == 0 ? 3 : mode == 1 ? 6 : mode == 2 ? 30 : 1000;
  int n = rnd(1, mode == 3 ? 120 : 40);
  bool big = it < 0;
  if (big) L0 = M, n = it == -1 ? 100000 : rnd(1, 20000); // contest size
  else if (it % 7 == 0) L0 = M;                          // small n, contest coordinates
  set<pll> s;
  if (it % 50 == 0) { // collinear
    ll dx = rnd(1, 3), dy = rnd(-3, 3), x0 = rnd(-L0, L0) / 2, y0 = rnd(-L0, L0) / 2;
    FOR (i, 0, n - 1) s.insert(pll(x0 + i * dx, y0 + i * dy));
  } else if ((big && -it % 3 == 0) || it % 11 == 0) {
    // (near-)cocircular: c + the 8 symmetries of (x, y), a quarter of them moved by 1
    ll cx0 = rnd(-M / 2, M / 2), cy0 = rnd(-M / 2, M / 2), spread = it % 2 ? M / 2 : 5;
    while (SZ(s) < n) {
      ll x = rnd(0, M / 4), y = rnd(0, M / 4), cx = cx0 + rnd(0, spread), cy = cy0 + rnd(0, spread);
      for (pll d : {pll(x, y), pll(-y, x), pll(-x, -y), pll(y, -x), pll(x, -y), pll(y, x), pll(-x, y), pll(-y, -x)})
        s.insert(pll(cx, cy) + d + pll(rnd(-1, 1), rnd(-1, 1)) * (rnd(0, 3) == 0));
      s.insert(pll(cx, cy));
    }
  } else FOR (i, 0, n - 1) s.insert(pll(rnd(-L0, L0), rnd(-L0, L0)));
  vector<pll> p(ALL(s));
  shuffle(ALL(p), rng);
  n = SZ(p);
  FOR (i, 0, n - 1) arr[i] = p[i];
  tool.init(n, arr.data());
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
  int h = hullBoundary(p);
  if (h < 0 || n <= 2) assert(SZ(ed) == n - 1);
  else assert(SZ(ed) == 3 * n - 3 - h);
  if (n <= 200) checkSmall(p, ed, h);
  if (n > 2 && h >= 0) checkBig(p, ed, h);
  return n;
}
int main() {
  int cnt = 0;
  vector<pll> arr(N);
  ll M[2] = {33000000, 1000000000};
  FOR (c, 0, 1) {
    int maxN = 0;
    FOR (it, 1, 2500) c ? run(ints::tool, M[c], it, arr) : run(tool, M[c], it, arr), cnt++;
    FOR (it, 1, 6) maxN = max(maxN, c ? run(ints::tool, M[c], -it, arr) : run(tool, M[c], -it, arr)), cnt++;
    printf("%s prelude, |coord| <= %lld: OK, largest n %d\n", c ? "integer" : "book", M[c], maxN);
  }
  printf("Delaunay_dq: %d point sets OK (planar, 3n-3-h edges, 2n-2-h empty-circumcircle faces; n up to 1e5)\n", cnt);
}
