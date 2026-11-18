// Minkowski(A, B) vs the hull of all pairwise sums of hull vertices (exact vector equality:
// both strictly convex, CCW, starting at the lexicographically smallest point).
// Precondition "|A|,|B| >= 3" is read as: hull(A), hull(B) have >= 3 points; the collinear
// (hull size 2) case is probed separately and reported, not asserted.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "8_Geometry/Convex_hull.cpp"
#include "8_Geometry/Minkowski_Sum.cpp"

// Reference: hull of all pairwise sums, computed in __int128 (sums reach 2 * |coord|).
typedef __int128 L;
L cr(pll o, pll a, pll b) { return (L)(a.X - o.X) * (b.Y - o.Y) - (L)(a.Y - o.Y) * (b.X - o.X); }
vector<pll> hull128(vector<pll> p) { // strictly convex, CCW, from the smallest point
  sort(ALL(p)), p.erase(unique(ALL(p)), p.end());
  if (SZ(p) <= 1) return {};
  vector<pll> h;
  FOR (s, 0, 1) {
    int st = SZ(h);
    for (auto q : p) {
      while (SZ(h) >= st + 2 && cr(h[SZ(h) - 2], h.back(), q) <= 0) h.pop_back();
      h.pb(q);
    }
    h.pop_back(), reverse(ALL(p));
  }
  return h;
}
vector<pll> brute(const vector<pll> &A, const vector<pll> &B) {
  vector<pll> C;
  for (auto a : hull128(A)) for (auto b : hull128(B)) C.pb(a + b);
  return hull128(C);
}
int main() {
  int cases = 0, degOK = 0, degBad = 0;
  FOR (it, 1, 12000) {
    int R = it % 3 ? 4 : 1000;
    auto gen = [&](int n) {
      vector<pll> P(n);
      for (auto &x : P) x = pll(rnd(-R, R), rnd(-R, R));
      if (rnd(0, 9) == 0) { ll a = rnd(-2, 2), b = rnd(-3, 3); for (auto &x : P) x.Y = a * x.X + b; }
      return P;
    };
    vector<pll> A = gen(rnd(3, it % 5 ? 8 : 30)), B = gen(rnd(3, it % 5 ? 8 : 30));
    vector<pll> hA = A, hB = B;
    hull(hA), hull(hB);
    bool deg = SZ(hA) < 3 || SZ(hB) < 3;
    vector<pll> want = brute(A, B);
    if (deg) { // outside the documented contract: just record
      (Minkowski(A, B) == want ? degOK : degBad)++;
      continue;
    }
    assert(Minkowski(A, B) == want);
    cases++;
  }
  // Contest size: |coord| <= 7.5e8 (the template's limit: it calls hull() on the sums, whose
  // ll cross needs |sum coord| <= ~1.5e9). Up to 1e5 input points, hulls up to ~300 vertices.
  const ll M = 750000000;
  int big = 0, maxOut = 0;
  { // worst case for hull(C): squares at the corners, sums span [-2M, 2M]^2
    vector<pll> A = {{-M, -M}, {M, -M}, {M, M}, {-M, M}}, B = {{0, -M}, {M, 0}, {0, M}, {-M, 0}};
    assert(Minkowski(A, A) == brute(A, A) && Minkowski(A, B) == brute(A, B)), big++;
  }
  FOR (it, 1, 100) {
    auto gen = [&](int n) {
      vector<pll> P(n);
      ll R = rnd(1, 4) == 1 ? rnd(1, M) : M, cx = rnd(-M + R, M - R), cy = rnd(-M + R, M - R);
      int k = rnd(3, it % 3 ? 300 : 10); // points on a circle -> up to ~k hull vertices
      FOR (i, 0, n - 1) {
        double a = rndd(0, 2 * M_PI), r = i < k ? R : rndd(0, R);
        P[i] = pll(clamp(cx + llround(r * cos(a)), -M, M), clamp(cy + llround(r * sin(a)), -M, M));
      }
      if (it % 10 == 0) FOR (i, 0, n - 1) P[i] = pll(rnd(-M, M), rnd(-M, M)); // few hull vertices
      return P;
    };
    vector<pll> A = gen(rnd(3, it % 2 ? 100000 : 1000)), B = gen(rnd(3, it % 2 ? 1000 : 100000));
    vector<pll> hA = hull128(A), hB = hull128(B);
    if (SZ(hA) < 3 || SZ(hB) < 3) continue;
    vector<pll> want = brute(hA, hB);
    assert(Minkowski(A, B) == want);
    maxOut = max(maxOut, SZ(want)), big++;
  }
  printf("Minkowski_Sum: %d small + %d |coord|<=7.5e8 cases OK (largest sum %d vertices; collinear input, not asserted: %d match, %d differ)\n",
         cases, big, maxOut, degOK, degBad);
}
