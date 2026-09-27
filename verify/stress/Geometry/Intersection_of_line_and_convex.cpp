// lineHull(a, b, C) (kactl LineHullIntersection with extrVertex -> cyc_tsearch).
// C = hull() output (strictly convex, CCW, n >= 3). Exact expected answer from s[i] = cmpL(i):
//   all s same nonzero sign        -> (-1, -1)
//   one zero, rest same sign       -> (i, -1)      touching corner i
//   s[i] = s[i+1] = 0, rest same   -> (i, i)       along side (i, i+1)
//   otherwise (both signs present) -> (i, j): crossed sides, a crossed corner k counts as side
//                                     (k, k+1); ordered by position along a -> b.
// Also checks TangentDir(C, dir) = argmax cross(dir, C[i]).
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "9_Else/cyc_tsearch.cpp"
#include "8_Geometry/Convex_hull.cpp"
#include "8_Geometry/Intersection_of_line_and_convex.cpp"

pii brute(pll a, pll b, const vector<pll> &C) {
  int n = SZ(C), pos = 0, neg = 0;
  vector<int> s(n);
  FOR (i, 0, n - 1) s[i] = sign(cross(C[i] - a, b - a)), pos += s[i] > 0, neg += s[i] < 0;
  if (pos == n || neg == n) return pii(-1, -1);
  if (!pos || !neg) {
    vector<int> z;
    FOR (i, 0, n - 1) if (!s[i]) z.pb(i);
    if (SZ(z) == 1) return pii(z[0], -1);
    assert(SZ(z) == 2); // strictly convex
    int i = (z[0] + 1) % n == z[1] ? z[0] : z[1];
    return pii(i, i);
  }
  vector<pair<long double, int>> hit;
  FOR (i, 0, n - 1) {
    int j = (i + 1) % n;
    long double t;
    if (!s[i]) t = dot(C[i] - a, b - a);
    else if (s[i] * s[j] < 0) {
      long double ci = cross(C[i] - a, b - a), cj = cross(C[j] - a, b - a);
      long double w = ci / (ci - cj); // point = C[i] + (C[j]-C[i]) * w
      t = dot(C[i] - a, b - a) + w * dot(C[j] - C[i], b - a);
    } else continue;
    hit.pb(t, i);
  }
  assert(SZ(hit) == 2);
  sort(ALL(hit));
  assert(hit[0].F < hit[1].F);
  return pii(hit[0].S, hit[1].S);
}
int main() {
  int cases = 0, kind[4] = {};
  FOR (it, 1, 60000) {
    int R = it % 3 ? 5 : 1000, n = rnd(3, it % 4 ? 10 : 60);
    vector<pll> C(n);
    for (auto &x : C) x = pll(rnd(-R, R), rnd(-R, R));
    hull(C);
    if (SZ(C) < 3) continue;
    rotate(C.begin(), C.begin() + rnd(0, SZ(C) - 1), C.end());
    FOR (k, 1, 10) {
      pll a, b;
      if (k <= 3) { // line through hull vertices (corner / side / diagonal cases)
        a = C[rnd(0, SZ(C) - 1)], b = C[rnd(0, SZ(C) - 1)];
        if (rnd(0, 1)) { pll d = b - a; b = a + d * 2; a = a - d; }
      } else a = pll(rnd(-R - 2, R + 2), rnd(-R - 2, R + 2)), b = pll(rnd(-R - 2, R + 2), rnd(-R - 2, R + 2));
      if (a == b) continue;
      // TangentDir
      pll dir = b - a;
      int td = TangentDir(C, dir);
      FOR (x, 0, SZ(C) - 1) assert(cross(dir, C[x]) <= cross(dir, C[td]));
      pii want = brute(a, b, C), got = lineHull(a, b, C);
      if (got != want) {
        printf("n=%d a=(%lld,%lld) b=(%lld,%lld) got (%d,%d) want (%d,%d)\nC:", SZ(C), a.X, a.Y, b.X, b.Y, got.F, got.S, want.F, want.S);
        for (auto [x, y] : C) printf(" (%lld,%lld)", x, y);
        puts(""); fflush(stdout);
        assert(0);
      }
      kind[want.F < 0 ? 0 : want.S < 0 ? 1 : want.F == want.S ? 2 : 3]++;
      cases++;
    }
  }
  printf("lineHull: %d queries OK (miss/corner/side/cross = %d/%d/%d/%d)\n", cases, kind[0], kind[1], kind[2], kind[3]);
}
