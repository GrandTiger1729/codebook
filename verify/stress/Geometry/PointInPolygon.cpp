// PointInPolygon on random star-shaped simple polygons (both orientations; consecutive
// vertices may be collinear) vs a brute force: -1 if p is on an edge, else the winding number (sum of
// atan2 angles) is +-1 inside, 0 outside. Points on vertices, edges and random.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "8_Geometry/PointInPolygon.cpp"
int main() {
  int cases = 0, inside = 0, boundary = 0;
  FOR (it, 1, 20000) {
    int n = rnd(3, 12), R = it % 2 ? 6 : 100;
    vector<pll> P;
    vector<pair<double, pll>> pts;
    pll c(0, 0);
    set<pll> seen;
    while ((int)pts.size() < n) {
      pll q(rnd(-R, R), rnd(-R, R));
      if (q == c || seen.count(q)) continue;
      seen.insert(q), pts.pb(atan2((double)q.Y, (double)q.X), q);
    }
    sort(ALL(pts));
    bool ok = true; // need strictly increasing angles for a simple star polygon
    FOR (i, 0, n - 1) {
      pll a = pts[i].S, b = pts[(i + 1) % n].S;
      ok = ok && cross(a, b) > 0;
    }
    if (!ok) continue;
    for (auto &[ang, q] : pts) P.pb(q);
    if (it % 2) reverse(ALL(P));
    FOR (k, 1, 20) {
      pll p(rnd(-R - 2, R + 2), rnd(-R - 2, R + 2));
      if (k % 5 == 0) p = P[rnd(0, n - 1)];
      int want;
      bool on = false;
      FOR (i, 0, n - 1) on |= btw(P[i], P[(i + 1) % n], p);
      if (on) want = -1;
      else {
        double w = 0;
        FOR (i, 0, n - 1) {
          pll a = P[i] - p, b = P[(i + 1) % n] - p;
          w += atan2((double)cross(a, b), (double)dot(a, b));
        }
        want = fabs(w) > 1 ? 1 : 0;
      }
      int got = PointInPolygon(P, p);
      assert(got == want);
      inside += got == 1, boundary += got == -1, cases++;
    }
  }
  printf("PointInPolygon ok: %d queries (%d inside, %d on the boundary)\n", cases, inside, boundary);
}
