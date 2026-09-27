#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
const int N = 1000;
#include "8_Geometry/point_in_circle.cpp"
bool seg_strict_intersect(pdd p1, pdd p2, pdd p3, pdd p4) { // not in the book, see Delaunay test
  return ori(p1, p2, p3) * ori(p1, p2, p4) < 0 && ori(p3, p4, p1) * ori(p3, p4, p2) < 0;
}
#include "8_Geometry/DelaunayTriangulation_dq.cpp"
#include "8_Geometry/Polar_Angle_Sort.cpp"
#include "8_Geometry/Half_plane_intersection.cpp"
#include "8_Geometry/Triangulation_Vonoroi.cpp"
// vec[u] = bisector half-planes (u's side on the left) with u's Delaunay neighbours.
// Check: box ∩ those half-planes == box ∩ bisectors with ALL other points (brute, by
// polygon clipping), both by clipping and via halfPlaneInter as the comment suggests.
vector<pdd> clip(const vector<pdd> &poly, pdd a, pdd b) { // keep left of a->b
  vector<pdd> r;
  FOR (i, 0, SZ(poly) - 1) {
    pdd p = poly[i], q = poly[(i + 1) % SZ(poly)];
    double cp = cross(b - a, p - a), cq = cross(b - a, q - a);
    if (cp >= 0) r.pb(p);
    if ((cp > 0 && cq < 0) || (cp < 0 && cq > 0)) r.pb(p + (q - p) * (cp / (cp - cq)));
  }
  return r;
}
double polyArea(const vector<pdd> &v) {
  double a = 0;
  FOR (i, 0, SZ(v) - 1) a += cross(v[i], v[(i + 1) % SZ(v)]);
  return a / 2;
}
pll arr[N];
int main() {
  int cnt = 0, cells = 0;
  FOR (it, 1, 1500) {
    int L = it % 3 == 0 ? 4 : it % 3 == 1 ? 12 : 500, n = rnd(1, it % 3 == 2 ? 60 : 25);
    set<pll> s;
    FOR (i, 1, n) s.insert(pll(2 * rnd(-L, L), 2 * rnd(-L, L))); // even coordinates
    vector<pll> p(ALL(s));
    shuffle(ALL(p), rng);
    n = SZ(p);
    FOR (i, 0, n - 1) arr[i] = p[i];
    build_voronoi_line(n, arr);
    ll B = 8 * L + 10;
    vector<pdd> box = {pdd(-B, -B), pdd(B, -B), pdd(B, B), pdd(-B, B)};
    double total = 0;
    FOR (u, 0, n - 1) {
      vector<pdd> a = box, b = box;
      for (auto l : vec[u]) a = clip(a, l.X, l.Y);
      FOR (v, 0, n - 1) if (v != u) {
        pdd pu(p[u].X, p[u].Y), pv(p[v].X, p[v].Y), m = (pu + pv) / 2;
        b = clip(b, m, m + perp(pv - pu));
      }
      double A = polyArea(a), Bb = polyArea(b);
      assert(fabs(A - Bb) < 1e-6 * max(1.0, Bb));
      assert(Bb > 0); // distinct points -> each cell has positive area
      total += Bb;
      // halfPlaneInter route
      vector<Line> ls = vec[u];
      FOR (i, 0, 3) ls.pb(box[i], box[(i + 1) % 4]);
      auto res = halfPlaneInter(ls);
      assert(SZ(res) >= 3);
      vector<pdd> w;
      FOR (i, 0, SZ(res) - 1) w.pb(intersect(res[i].X, res[i].Y, res[(i + 1) % SZ(res)].X, res[(i + 1) % SZ(res)].Y));
      assert(fabs(polyArea(w) - Bb) < 1e-6 * max(1.0, Bb));
      cells++;
    }
    assert(fabs(total - 4.0 * B * B) < 1e-6 * total); // cells tile the box
    cnt++;
  }
  printf("Triangulation_Vonoroi: %d point sets / %d cells OK vs all-pairs bisector clipping (+ halfPlaneInter)\n", cnt, cells);
}
