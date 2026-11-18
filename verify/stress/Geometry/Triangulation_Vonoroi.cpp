#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
const int N = 100000;
#include "8_Geometry/point_in_circle.cpp"
bool seg_strict_intersect(pdd p1, pdd p2, pdd p3, pdd p4) { // not in the book, see Delaunay test
  return ori(p1, p2, p3) * ori(p1, p2, p4) < 0 && ori(p3, p4, p1) * ori(p3, p4, p2) < 0;
}
#include "8_Geometry/DelaunayTriangulation_dq.cpp"
#include "8_Geometry/Polar_Angle_Sort.cpp"
#include "8_Geometry/Half_plane_intersection.cpp"
#include "8_Geometry/Triangulation_Vonoroi.cpp"
// Bisector lines run m -> m + perp(v - u) with |m + perp| <= 3 |coord|. Within one cell
// halfPlaneInter's crosses (area_pair, sort's ori) stay <= 6 |coord|^2 between bisectors and
// <= 8 |coord|^2 against a box of the same size (worst case found by a grid search).
// Book prelude: Line = pdd, so those crosses are doubles, exact while 8 |coord|^2 < 2^53
// (|coord| <= ~3.3e7). Integer prelude (Default_code_int, not in the book): ll, fine for
// |coord| <= 1e9 (8e18 < 2^63).
namespace ints {
#include "8_Geometry/Default_code_int.cpp"
#include "8_Geometry/point_in_circle.cpp"
bool seg_strict_intersect(pll p1, pll p2, pll p3, pll p4) {
  return ori(p1, p2, p3) * ori(p1, p2, p4) < 0 && ori(p3, p4, p1) * ori(p3, p4, p2) < 0;
}
#include "8_Geometry/DelaunayTriangulation_dq.cpp"
#include "8_Geometry/Polar_Angle_Sort.cpp"
#include "8_Geometry/Half_plane_intersection.cpp"
#include "8_Geometry/Triangulation_Vonoroi.cpp"
}
// vec[u] = bisector half-planes (u's side on the left) with u's Delaunay neighbours.
// Check: box ∩ those half-planes == box ∩ bisectors with ALL other points (brute, by
// polygon clipping), both by clipping and via halfPlaneInter as the comment suggests.
// n = 1e5: no brute; the cells (via halfPlaneInter) must have positive area and tile the box.
typedef long double D;
typedef pair<D, D> pD;
pD operator-(pD a, pD b) { return pD(a.X - b.X, a.Y - b.Y); }
pD operator+(pD a, pD b) { return pD(a.X + b.X, a.Y + b.Y); }
pD operator*(pD a, D k) { return pD(a.X * k, a.Y * k); }
D crs(pD a, pD b) { return a.X * b.Y - a.Y * b.X; }
template <class P> pD toD(P p) { return pD(p.X, p.Y); }
vector<pD> clip(const vector<pD> &poly, pD a, pD b) { // keep left of a->b
  vector<pD> r;
  FOR (i, 0, SZ(poly) - 1) {
    pD p = poly[i], q = poly[(i + 1) % SZ(poly)];
    D cp = crs(b - a, p - a), cq = crs(b - a, q - a);
    if (cp >= 0) r.pb(p);
    if ((cp > 0 && cq < 0) || (cp < 0 && cq > 0)) r.pb(p + (q - p) * (cp / (cp - cq)));
  }
  return r;
}
D polyArea(const vector<pD> &v) {
  D a = 0;
  FOR (i, 0, SZ(v) - 1) a += crs(v[i], v[(i + 1) % SZ(v)]);
  return a / 2;
}
pD meet(pD p1, pD p2, pD p3, pD p4) { // p3 + t (p4 - p3): no cancellation of C^3 terms
  D a = crs(p2 - p1, p3 - p1), b = crs(p2 - p1, p4 - p1);
  return p3 + (p4 - p3) * (a / (a - b));
}
int cells;
// Vec = vector<vector<Line>> of the config, hpi = its halfPlaneInter
template <class Vec, class F> void check(const vector<pll> &p, const Vec &vec, F hpi, ll B, bool brute) {
  int n = SZ(p);
  typedef typename Vec::value_type::value_type Ln;
  typedef decltype(Ln().X) Pt;
  vector<Pt> corner = {Pt(-B, -B), Pt(B, -B), Pt(B, B), Pt(-B, B)};
  vector<pD> box;
  for (auto c : corner) box.pb(toD(c));
  D total = 0;
  FOR (u, 0, n - 1) {
    D Bb = 0;
    if (brute) {
      vector<pD> a = box, b = box;
      for (auto l : vec[u]) a = clip(a, toD(l.X), toD(l.Y));
      FOR (v, 0, n - 1) if (v != u) {
        pD pu = toD(p[u]), pv = toD(p[v]), m = (pu + pv) * 0.5L, d = pv - pu;
        b = clip(b, m, m + pD(-d.Y, d.X));
      }
      D A = polyArea(a);
      Bb = polyArea(b);
      assert(fabsl(A - Bb) < 1e-9L * max<D>(1, Bb));
      assert(Bb > 0); // distinct points -> each cell has positive area
    }
    // halfPlaneInter route
    auto ls = vec[u];
    FOR (i, 0, 3) ls.pb(corner[i], corner[(i + 1) % 4]);
    auto res = hpi(ls);
    assert(SZ(res) >= 3);
    vector<pD> w;
    FOR (i, 0, SZ(res) - 1) w.pb(meet(toD(res[i].X), toD(res[i].Y), toD(res[(i + 1) % SZ(res)].X), toD(res[(i + 1) % SZ(res)].Y)));
    D A = polyArea(w);
    if (brute) assert(fabsl(A - Bb) < 1e-9L * max<D>(1, Bb));
    else assert(A > 0);
    total += A, cells++;
  }
  assert(fabsl(total - 4.0L * B * B) < 1e-9L * total); // cells tile the box
}
// L < 0: extreme points, coordinates from {0, +-(2|L| - 2), +-2|L|} (the worst-case crosses)
template <class Pt> vector<pll> genPoints(ll L, int n, vector<Pt> &arr) {
  set<pll> s;
  ll ext[5] = {2 * L, 2 * L + 2, 0, -2 * L - 2, -2 * L};
  FOR (i, 1, n) s.insert(L < 0 ? pll(ext[rnd(0, 4)], ext[rnd(0, 4)]) : pll(2 * rnd(-L, L), 2 * rnd(-L, L))); // even
  vector<pll> p(ALL(s));
  shuffle(ALL(p), rng);
  FOR (i, 0, SZ(p) - 1) arr[i] = Pt(p[i].X, p[i].Y);
  return p;
}
int main() {
  int cnt = 0;
  vector<pll> arr(N);
  auto bookHPI = [](vector<Line> ls) { return halfPlaneInter(ls); };
  auto intHPI = [](vector<ints::Line> ls) { return ints::halfPlaneInter(ls); };
  auto runBook = [&](ll L, int n, ll B, bool brute) {
    auto p = genPoints(L, n, arr);
    build_voronoi_line(SZ(p), arr.data()), check(p, vec, bookHPI, B, brute), cnt++;
  };
  auto runInt = [&](ll L, int n, ll B, bool brute) {
    auto p = genPoints(L, n, arr);
    ints::build_voronoi_line(SZ(p), arr.data()), check(p, ints::vec, intHPI, B, brute), cnt++;
  };
  ll Mbook = 33000000, Mint = 1000000000; // max |coord| (even), box |x|, |y| <= M
  FOR (it, 1, 1500) { // small, as before: box 4x the points
    ll L = it % 3 == 0 ? 4 : it % 3 == 1 ? 12 : 500, n = rnd(1, it % 3 == 2 ? 60 : 25);
    it % 2 ? runBook(L, n, 8 * L + 10, true) : runInt(L, n, 8 * L + 10, true);
  }
  FOR (it, 1, 400) { // contest coordinates
    ll M = it % 2 ? Mbook : Mint, L = rnd(1, 4) == 1 ? rnd(1, M / 2) : M / 2;
    if (it % 5 == 0) L = -M / 2;
    int n = rnd(1, it % 20 ? 60 : 600);
    it % 2 ? runBook(L, n, M, true) : runInt(L, n, M, true);
  }
  runBook(Mbook / 2, 100000, Mbook, false), runInt(Mint / 2, 100000, Mint, false); // n = 1e5
  printf("Triangulation_Vonoroi: %d point sets / %d cells OK vs all-pairs bisector clipping (+ halfPlaneInter); |coord| <= %lld (book prelude) / %lld (integer), n up to 1e5\n",
         cnt, cells, Mbook, Mint);
}
