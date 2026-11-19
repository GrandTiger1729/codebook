// PointInPolygon on random star-shaped simple polygons (both orientations; consecutive
// vertices may be collinear) vs a brute force: -1 if p is on an edge, else the winding number (sum of
// atan2 angles) is +-1 inside, 0 outside. Points on vertices, edges and random.
// Second phase: |coord| <= 1e9, n up to 1e5, queries on / next to edges; reference in __int128.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "8_Geometry/PointInPolygon.cpp"
typedef __int128 L;
bool onSeg(pll a, pll b, pll p) { // p on closed segment ab, exact
  pll u = a - p, v = b - p;
  return (L)u.X * v.Y == (L)u.Y * v.X && (L)u.X * v.X + (L)u.Y * v.Y <= 0;
}
int bigBrute(const vector<pll> &P, pll p) {
  int n = SZ(P);
  FOR (i, 0, n - 1) if (onSeg(P[i], P[(i + 1) % n], p)) return -1;
  long double w = 0;
  FOR (i, 0, n - 1) {
    pll a = P[i] - p, b = P[(i + 1) % n] - p;
    w += atan2l((long double)((L)a.X * b.Y - (L)a.Y * b.X), (long double)((L)a.X * b.X + (L)a.Y * b.Y));
  }
  return fabsl(w) > 1;
}
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
  const ll M = 1e9;
  int bigCases = 0;
  FOR (it, 1, 40) {
    int n = rnd(3, it % 4 ? 100000 : 30);
    ll c = it % 2 ? 0 : rnd(-M / 2, M / 2), R = M - llabs(c);
    vector<pair<long double, pll>> pts;
    set<pll> seen;
    while (SZ(pts) < n) {
      long double a = rndd(0, 2 * M_PI), r = it % 3 ? R - rnd(0, 1000) : rnd(1, R);
      pll q(c + llroundl(r * cosl(a)), c + llroundl(r * sinl(a)));
      if (q == pll(c, c) || seen.count(q)) continue;
      seen.insert(q), pts.pb(atan2l(q.Y - c, q.X - c), q);
    }
    sort(ALL(pts));
    vector<pll> P;
    bool ok = true;
    FOR (i, 0, n - 1) {
      pll a = pts[i].S - pll(c, c), b = pts[(i + 1) % n].S - pll(c, c);
      ok = ok && (L)a.X * b.Y - (L)a.Y * b.X > 0;
    }
    if (!ok) { it--; continue; }
    for (auto &[ang, q] : pts) P.pb(q);
    if (it % 2) reverse(ALL(P));
    FOR (k, 1, 1000000 / n) {
      int i = rnd(0, n - 1), t = rnd(0, 4);
      pll a = P[i], d = P[(i + 1) % n] - a, p(rnd(-M, M), rnd(-M, M));
      ll g = __gcd(llabs(d.X), llabs(d.Y));
      if (t == 0) p = a;
      if (t == 1 || t == 2) p = a + d / g * rnd(0, g);
      if (t == 2) p = p + pll(rnd(-1, 1), rnd(-1, 1));
      int got = PointInPolygon(P, p);
      assert(got == bigBrute(P, p));
      inside += got == 1, boundary += got == -1, bigCases++;
    }
  }
  printf("PointInPolygon ok: %d small + %d |coord|<=1e9 queries (%d inside, %d on the boundary)\n", cases, bigCases, inside, boundary);
}
