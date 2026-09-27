#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/Intersection_of_polygon_and_circle.cpp"
// area_poly_circle(poly, O, r): |poly ∩ disk|, poly simple, either orientation.
// Reference: kactl CirclePolygonIntersection.h (atan2-based), ported to pdd.
double kactl(pdd c, double r, vector<pdd> ps) {
  auto arg = [](pdd p, pdd q) { return atan2(cross(p, q), dot(p, q)); };
  auto tri = [&](pdd p, pdd q) {
    double r2 = r * r / 2;
    pdd d = q - p;
    double a = dot(d, p) / abs2(d), b = (abs2(p) - r * r) / abs2(d), det = a * a - b;
    if (det <= 0) return arg(p, q) * r2;
    double s = max(0., -a - sqrt(det)), t = min(1., -a + sqrt(det));
    if (t < 0 || 1 <= s) return arg(p, q) * r2;
    pdd u = p + d * s, v = q + d * (t - 1);
    return arg(p, u) * r2 + cross(u, v) / 2 + arg(v, q) * r2;
  };
  double sum = 0;
  FOR (i, 0, SZ(ps) - 1) sum += tri(ps[i] - c, ps[(i + 1) % SZ(ps)] - c);
  return fabs(sum);
}
vector<pdd> starPoly(vector<pdd> pts) { // simple polygon, star-shaped around a random centre
  sort(ALL(pts)), pts.erase(unique(ALL(pts)), pts.end());
  pdd c(0, 0);
  for (pdd p : pts) c = c + p;
  c = c / SZ(pts) + pdd(rndd(-1e-3, 1e-3), rndd(-1e-3, 1e-3));
  sort(ALL(pts), [&](pdd a, pdd b) { return atan2(a.Y - c.Y, a.X - c.X) < atan2(b.Y - c.Y, b.X - c.X); });
  return pts;
}
int main() {
  int cnt = 0, nanCnt = 0; double worst = 0;
  vector<pdd> firstBad; pdd badO; double badR = 0;
  FOR (it, 1, 100000) {
    int mode = it % 4, L = mode == 2 ? 20 : mode == 3 ? 60 : 5;
    int n = rnd(3, 9);
    vector<pdd> pts(n);
    for (auto &p : pts) p = mode != 2 ? pdd(rnd(0, L), rnd(0, L)) : pdd(rndd(0, L), rndd(0, L));
    auto poly = starPoly(pts);
    if (SZ(poly) < 3) continue;
    if (rnd(0, 1)) reverse(ALL(poly));
    pdd O = mode < 2 ? pdd(rnd(0, L), rnd(0, L)) : pdd(rndd(0, L), rndd(0, L));
    double r = mode == 0 ? rnd(0, 2 * L) : mode == 1 ? sqrt(rnd(0, 2 * L * L)) : rndd(0, L);
    if (mode == 3) { // centre on the line of an edge (sign(cross) == 0 for that edge)
      int i = rnd(0, SZ(poly) - 1); pdd a = poly[i], b = poly[(i + 1) % SZ(poly)];
      O = rnd(0, 1) ? a - (b - a) * (double)rnd(1, 2) : a + (b - a) * 0.5;
    }
    double got = area_poly_circle(poly, O, r), want = kactl(O, r, poly);
    cnt++;
    if (isnan(got)) {
      if (!nanCnt++) firstBad = poly, badO = O, badR = r;
      continue;
    }
    worst = max(worst, fabs(got - want));
    if (fabs(got - want) > 1e-6) {
      printf("mismatch got=%.10f want=%.10f r=%g O=(%g,%g) poly:", got, want, r, O.X, O.Y);
      for (pdd p : poly) printf(" (%g,%g)", p.X, p.Y);
      puts(""); fflush(stdout); assert(0);
    }
  }
  if (nanCnt) {
    printf("NaN in %d/%d cases, first: r=%g O=(%g,%g) poly:", nanCnt, cnt, badR, badO.X, badO.Y);
    for (pdd p : firstBad) printf(" (%g,%g)", p.X, p.Y);
    puts(""); fflush(stdout); assert(0);
  }
  printf("area_poly_circle: %d cases OK vs kactl circlePoly, max err %.2e\n", cnt, worst);
}
