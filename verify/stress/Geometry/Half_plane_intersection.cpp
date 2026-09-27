#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/Polar_Angle_Sort.cpp" // cmp() (commented out of content.tex)
#include "8_Geometry/Half_plane_intersection.cpp"
// halfPlaneInter(lines): half-plane = left of Line.X -> Line.Y; integer coordinates
// (cmp/area_pair go through pll). Result size <= 2 means "no (positive-area) region".
// Brute: all pairwise line intersections that lie in every half-plane -> hull area.
double polyArea(vector<pdd> v) {
  double a = 0;
  FOR (i, 0, SZ(v) - 1) a += cross(v[i], v[(i + 1) % SZ(v)]);
  return a / 2;
}
bool inside(Line l, pdd p, double e) { return cross(l.Y - l.X, p - l.X) >= -e * abs(l.Y - l.X); }
double hullArea(vector<pdd> p) {
  sort(ALL(p));
  if (SZ(p) < 3) return 0;
  vector<pdd> h(2 * SZ(p)); int k = 0;
  FOR (s, 0, 1) {
    int st = k;
    for (auto q : p) { while (k >= st + 2 && cross(h[k - 1] - h[k - 2], q - h[k - 2]) <= 1e-12) k--; h[k++] = q; }
    k--; reverse(ALL(p));
  }
  h.resize(max(k, 0));
  return polyArea(h);
}
// returns -1 if no feasible point, else area of the intersection
double brute(const vector<Line> &ls) {
  vector<pdd> pts;
  FOR (i, 0, SZ(ls) - 1) FOR (j, i + 1, SZ(ls) - 1) {
    if (fabs(cross(ls[i].Y - ls[i].X, ls[j].Y - ls[j].X)) < 1e-12) continue;
    pdd p = intersect(ls[i].X, ls[i].Y, ls[j].X, ls[j].Y);
    if (all_of(ALL(ls), [&](Line l) { return inside(l, p, 1e-9); })) pts.pb(p);
  }
  if (pts.empty()) return -1;
  return hullArea(pts);
}
int main() {
  int cnt = 0, stat[3] = {}, degenNonEmpty = 0;
  FOR (it, 1, 25000) {
    int mode = it % 3, L = mode == 0 ? 3 : mode == 1 ? 10 : 1000, B = mode == 2 ? 2000 : 2 * L;
    int n = rnd(1, mode == 2 ? 12 : 8);
    vector<Line> ls;
    FOR (i, 1, n) {
      pdd p(rnd(-L, L), rnd(-L, L)), q(rnd(-L, L), rnd(-L, L));
      if (p == q) { i--; continue; }
      ls.pb(p, q);
      if (rnd(0, 5) == 0) ls.pb(q, p);                                         // opposite half-plane
      if (rnd(0, 5) == 0) { double k = rnd(1, 2); ls.pb(p + (q - p) * k, q + (q - p) * k); } // same line, other points
      if (rnd(0, 5) == 0) { pdd d(rnd(-2, 2), rnd(-2, 2)); ls.pb(p + d, q + d); } // parallel
    }
    for (auto [a, b] : vector<pair<pdd, pdd>>{{{-B, -B}, {B, -B}}, {{B, -B}, {B, B}}, {{B, B}, {-B, B}}, {{-B, B}, {-B, -B}}})
      ls.pb(a, b);
    shuffle(ALL(ls), rng);
    auto res = halfPlaneInter(ls);
    double want = brute(ls);
    cnt++;
    if (want > 1e-7) {
      stat[2]++;
      if (SZ(res) < 3) { printf("missing region, want %.6f\n", want); fflush(stdout); assert(0); }
      vector<pdd> v;
      FOR (i, 0, SZ(res) - 1) { auto a = res[i], b = res[(i + 1) % SZ(res)]; v.pb(intersect(a.X, a.Y, b.X, b.Y)); }
      for (auto p : v) for (auto l : ls) assert(inside(l, p, 1e-6));
      double got = polyArea(v);
      if (fabs(got - want) > 1e-6 * max(1.0, want)) { printf("area got %.9f want %.9f\n", got, want); fflush(stdout); assert(0); }
    } else {
      stat[want < 0 ? 0 : 1]++;
      if (want < 0) assert(SZ(res) <= 2);  // truly empty
      else if (SZ(res) > 2) degenNonEmpty++; // point / segment
    }
  }
  printf("halfPlaneInter: %d cases OK (empty %d, zero-area %d [size>2 returned %d], positive %d)\n",
         cnt, stat[0], stat[1], degenNonEmpty, stat[2]);
}
