// PointInConvex(C, p, strict): C = output of hull() (strictly convex, CCW, may be size 1/2),
// also tested reversed (CW) and rotated. Reference: O(n) orientation test.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "8_Geometry/Convex_hull.cpp"
#include "8_Geometry/PointInConvex.cpp"

// Reference in __int128 (the template's ll cross is what is under test).
typedef __int128 L;
int sg(L x) { return (x > 0) - (x < 0); }
int ori128(pll a, pll b, pll c) { return sg((L)(b.X - a.X) * (c.Y - a.Y) - (L)(b.Y - a.Y) * (c.X - a.X)); }
bool onSeg(pll a, pll b, pll p) { // p on closed segment ab
  return !ori128(a, b, p) && min(a.X, b.X) <= p.X && p.X <= max(a.X, b.X) && min(a.Y, b.Y) <= p.Y && p.Y <= max(a.Y, b.Y);
}
bool brute(const vector<pll> &C, pll p, bool strict) {
  int n = SZ(C);
  if (n == 1) return !strict && C[0] == p;
  if (n == 2) return !strict && onSeg(C[0], C[1], p);
  int s = ori128(C[0], C[1], C[2]); // orientation of the polygon
  FOR (i, 0, n - 1) {
    int o = ori128(C[i], C[(i + 1) % n], p) * s;
    if (o < 0 || (strict && o == 0)) return false;
  }
  return true;
}
// Contest size: |coord| <= 1e9, hull of up to 2e5 points near a circle (large hulls), points
// collinear along a random lattice direction, queries on / next to edges and vertices.
const ll M = 1e9;
pll clampP(pll p) { return pll(clamp(p.X, -M, M), clamp(p.Y, -M, M)); }
vector<pll> bigPoints(int it) {
  int n = rnd(1, it % 4 ? 200000 : 50);
  vector<pll> P(n);
  if (it % 5 == 0) { // collinear: base + t * dir
    pll d(rnd(-1000, 1000), rnd(-1000, 1000));
    if (d == pll(0, 0)) d = pll(1, 1);
    ll tmax = M / max({llabs(d.X), llabs(d.Y), 1LL});
    for (auto &x : P) { ll t = rnd(-tmax, tmax); x = pll(d.X * t, d.Y * t); }
  } else if (it % 5 == 1) {
    for (auto &x : P) x = pll(rnd(-M, M), rnd(-M, M));
  } else { // near a circle (or an ellipse squeezed by an integer factor)
    ll sq = rnd(1, 3);
    for (auto &x : P) {
      double a = rndd(0, 2 * M_PI), r = M - rnd(0, 3);
      x = clampP(pll(llround(r * cos(a)), llround(r * sin(a)) / sq));
    }
  }
  return P;
}
pll bigQuery(const vector<pll> &C) {
  int n = SZ(C), i = rnd(0, n - 1), t = rnd(0, 5);
  pll a = C[i], b = C[(i + 1) % n], d = b - a;
  ll g = __gcd(llabs(d.X), llabs(d.Y));
  if (t == 0) return a;
  pll p = a;
  if (g) p = a + d / g * rnd(0, g); // lattice point on edge
  if (t == 1) return p;
  if (t <= 3) return clampP(p + pll(rnd(-1, 1), rnd(-1, 1))); // just off the edge
  if (t == 4) return pll(rnd(-M, M), rnd(-M, M));
  return clampP(C[rnd(0, n - 1)] / 2LL + C[rnd(0, n - 1)] / 2LL + pll(rnd(-1, 1), rnd(-1, 1)));
}
int main() {
  int cases = 0, sz[4] = {};
  FOR (it, 1, 100000) {
    int R = it % 3 ? 4 : 30, n = rnd(1, it % 5 ? 12 : 40);
    vector<pll> P(n);
    for (auto &x : P) x = pll(rnd(-R, R), rnd(-R, R));
    if (it % 7 == 0) for (auto &x : P) x.Y = x.X * 2 + 1; // all collinear -> hull of size 2
    vector<pll> C = P;
    hull(C);
    if (C.empty()) C = {P[0]}; // hull() drops a lone point; PointInConvex handles SZ==1
    if (it % 2) reverse(ALL(C));
    rotate(C.begin(), C.begin() + rnd(0, SZ(C) - 1), C.end());
    sz[min(SZ(C), 3)]++;
    FOR (k, 1, 20) {
      pll p = rnd(0, 3) ? pll(rnd(-R - 1, R + 1), rnd(-R - 1, R + 1)) : C[rnd(0, SZ(C) - 1)];
      FOR (st, 0, 1) assert(PointInConvex(C, p, st) == brute(C, p, st)), cases++;
    }
  }
  int bigCases = 0, maxHull = 0;
  FOR (it, 1, 60) {
    vector<pll> P = bigPoints(it), C = P;
    hull(C);
    if (C.empty()) C = {P[0]};
    if (it % 2) reverse(ALL(C));
    rotate(C.begin(), C.begin() + rnd(0, SZ(C) - 1), C.end());
    maxHull = max(maxHull, SZ(C));
    int q = max(200, 1000000 / SZ(C)); // brute is O(|C|) per query
    FOR (k, 1, q) {
      pll p = bigQuery(C);
      FOR (st, 0, 1) assert(PointInConvex(C, p, st) == brute(C, p, st)), bigCases++;
    }
  }
  printf("PointInConvex: %d small + %d |coord|<=1e9 queries OK (hull sizes 1/2/>=3: %d/%d/%d, largest big hull %d)\n",
         cases, bigCases, sz[1], sz[2], sz[3], maxHull);
}
