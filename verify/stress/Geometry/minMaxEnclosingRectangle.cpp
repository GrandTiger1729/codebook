// solve(dots) = (min, max) area over all rectangles circumscribing the point set (UVA 819).
// Reference: area(t) = width(t) * height(t) of the bounding box in the frame rotated by t,
// period pi/2. Min is attained at an edge direction (checked exactly on every edge);
// max by a dense scan + golden-section refinement, in long double.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
double abs(pll a) { return sqrt((double)abs2(a)); } // not in Default_code_int; template needs it
#include "stress.h"
#include "8_Geometry/Convex_hull.cpp"
#include "8_Geometry/minMaxEnclosingRectangle.cpp"

typedef long double ld;
ld area(const vector<pll> &P, ld t) {
  ld c = cosl(t), s = sinl(t), a = 1e30, b = -1e30, e = 1e30, f = -1e30;
  for (auto [x, y] : P) {
    ld u = x * c + y * s, v = -x * s + y * c;
    a = min(a, u), b = max(b, u), e = min(e, v), f = max(f, v);
  }
  return (b - a) * (f - e);
}
pair<ld, ld> brute(vector<pll> P) {
  hull(P);
  int n = SZ(P);
  ld mn = 1e30, mx = 0, PI2 = acosl(-1) / 2;
  FOR (i, 0, n - 1) {
    pll d = P[(i + 1) % n] - P[i];
    mn = min(mn, area(P, atan2l(d.Y, d.X)));
  }
  // area is piecewise smooth; grid scan, then golden-section around the top few grid points
  const int K = 720;
  vector<pair<ld, int>> cand;
  FOR (i, 0, K - 1) cand.pb(area(P, PI2 * i / K), i);
  partial_sort(cand.begin(), cand.begin() + 4, cand.end(), greater<>());
  mx = cand[0].F;
  FOR (c, 0, 3) {
    ld lo = PI2 * (cand[c].S - 1) / K, hi = PI2 * (cand[c].S + 1) / K;
    FOR (r, 1, 60) {
      ld m1 = lo + (hi - lo) * 0.381966, m2 = hi - (hi - lo) * 0.381966;
      if (area(P, m1) < area(P, m2)) lo = m1; else hi = m2;
    }
    mx = max(mx, area(P, (lo + hi) / 2));
  }
  return {mn, mx};
}
int main() {
  int cases = 0;
  FOR (it, 1, 3000) {
    int R = it % 3 ? 6 : 1000, n = rnd(3, it % 4 ? 10 : 40);
    vector<pll> P(n);
    for (auto &x : P) x = pll(rnd(-R, R), rnd(-R, R));
    { vector<pll> H = P; hull(H); if (SZ(H) < 3) continue; } // documented? see report
    shuffle(ALL(P), rng);
    auto [wmn, wmx] = brute(P);
    vector<pll> Q = P;
    pdd got = solve(Q);
    ld tol = 1e-6 * max((ld)1, wmx);
    if (fabsl(got.X - wmn) > tol || fabsl(got.Y - wmx) > tol) {
      printf("got (%.9f, %.9f) want (%.9Lf, %.9Lf)\nP:", got.X, got.Y, wmn, wmx);
      for (auto [x, y] : P) printf(" (%lld,%lld)", x, y);
      puts(""); fflush(stdout);
      assert(0);
    }
    cases++;
  }
  printf("minMaxEnclosingRectangle: %d point sets OK\n", cases);
}
