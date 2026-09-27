// polyUnion(polys): area of the union of simple CCW polygons. Reference: vertical slab
// decomposition (cut at every vertex x and every edge-edge intersection x; inside a slab the
// union's cross-section length is linear, so area = width * length at the slab middle).
// Inputs: small integer coords so shared edges (same / opposite direction), identical copies,
// touching vertices and containment are frequent.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "8_Geometry/Convex_hull.cpp"
#include "8_Geometry/PolyUnion.cpp"

typedef long double ld;
ld brute(const vector<vector<pll>> &P) {
  vector<pair<pll, pll>> E;
  vector<ld> xs;
  for (auto &p : P) FOR (i, 0, SZ(p) - 1) E.pb(p[i], p[(i + 1) % SZ(p)]), xs.pb(p[i].X);
  FOR (i, 0, SZ(E) - 1) FOR (j, 0, i - 1) {
    auto [a, b] = E[i]; auto [c, d] = E[j];
    ll den = cross(b - a, d - c);
    if (!den) continue;
    ld t = (ld)cross(c - a, d - c) / den, u = (ld)cross(c - a, b - a) / den;
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) xs.pb(a.X + t * (b.X - a.X));
  }
  sort(ALL(xs));
  ld res = 0;
  FOR (k, 1, SZ(xs) - 1) {
    ld x0 = xs[k - 1], x1 = xs[k];
    if (x1 - x0 < 1e-12) continue;
    ld xm = (x0 + x1) / 2;
    vector<pair<ld, ld>> iv;
    for (auto &p : P) {
      vector<ld> ys;
      FOR (i, 0, SZ(p) - 1) {
        pll a = p[i], b = p[(i + 1) % SZ(p)];
        if ((a.X < xm) != (b.X < xm)) ys.pb(a.Y + (b.Y - a.Y) * (xm - a.X) / (b.X - a.X));
      }
      sort(ALL(ys));
      assert(SZ(ys) % 2 == 0);
      for (int i = 0; i < SZ(ys); i += 2) iv.pb(ys[i], ys[i + 1]);
    }
    sort(ALL(iv));
    ld len = 0, hi = -1e18;
    for (auto [l, r] : iv) {
      if (r <= hi) continue;
      len += r - max(l, hi), hi = r;
    }
    res += len * (x1 - x0);
  }
  return res;
}
ll area2(const vector<pll> &p) {
  ll s = 0;
  FOR (i, 0, SZ(p) - 1) s += cross(p[i], p[(i + 1) % SZ(p)]);
  return s;
}
int R;
pll g() { return pll(rnd(-R, R), rnd(-R, R)); }
vector<pll> genPoly(int type) {
  vector<pll> p;
  if (type == 0) { // triangle
    p = {g(), g(), g()};
  } else if (type == 1) { // axis-parallel rectangle -> many shared / overlapping edges
    pll a = g(), b = g();
    if (a.X > b.X) swap(a.X, b.X);
    if (a.Y > b.Y) swap(a.Y, b.Y);
    p = {a, pll(b.X, a.Y), b, pll(a.X, b.Y)};
  } else if (type == 2) { // convex hull
    int n = rnd(3, 8);
    FOR (i, 1, n) p.pb(g());
    hull(p);
  } else { // star-shaped around a center, vertices sorted by exact angle
    pll c = g();
    int n = rnd(3, 8);
    FOR (i, 1, n) { pll q = g(); if (q != c) p.pb(q - c); }
    auto half = [](pll v) { return v.Y < 0 || (v.Y == 0 && v.X < 0); };
    sort(ALL(p), [&](pll a, pll b) {
      return half(a) != half(b) ? half(a) < half(b) : cross(a, b) > 0;
    });
    FOR (i, 1, SZ(p) - 1) { // same angle from the center -> not simple, reject
      pll a = p[i - 1], b = p[i];
      if (cross(a, b) == 0 && dot(a, b) > 0) return {};
    }
    for (auto &q : p) q = q + c;
    if (SZ(p) >= 3 && cross(p.back() - c, p[0] - c) == 0 && dot(p.back() - c, p[0] - c) > 0) return {};
    if (SZ(p) >= 3) { // the center must be strictly inside the kernel: every edge sees it CCW
      FOR (i, 0, SZ(p) - 1) if (ori(p[i], p[(i + 1) % SZ(p)], c) <= 0) return {};
    }
  }
  if (SZ(p) < 3) return {};
  // no repeated vertices, no zero-length edges
  vector<pll> s = p; sort(ALL(s));
  if (unique(ALL(s)) != s.end()) return {};
  ll a = area2(p);
  if (a == 0) return {};
  if (a < 0) reverse(ALL(p));
  return p;
}
int main() {
  int cases = 0, dup = 0;
  FOR (it, 1, 20000) {
    R = it % 4 ? 3 : 10;
    int k = rnd(1, it % 3 ? 3 : 5);
    vector<vector<pll>> P;
    while (SZ(P) < k) {
      if (!P.empty() && rnd(0, 5) == 0) { // exact copy (possibly rotated) of an earlier polygon
        auto q = P[rnd(0, SZ(P) - 1)];
        rotate(q.begin(), q.begin() + rnd(0, SZ(q) - 1), q.end());
        P.pb(q), dup++;
        continue;
      }
      auto q = genPoly(rnd(0, 3));
      if (!q.empty()) P.pb(q);
    }
    assert(fabsl(brute({P[0]}) - area2(P[0]) / 2.0L) < 1e-9); // reference self-check
    ld want = brute(P);
    double got = polyUnion(P);
    if (fabsl(got - want) > 1e-6 * max((ld)1, want)) {
      printf("got %.9f want %.9Lf\n", got, want);
      for (auto &p : P) { for (auto [x, y] : p) printf("(%lld,%lld) ", x, y); puts(""); }
      fflush(stdout);
      assert(0);
    }
    cases++;
  }
  printf("PolyUnion: %d polygon sets OK (%d duplicated polygons)\n", cases, dup);
}
