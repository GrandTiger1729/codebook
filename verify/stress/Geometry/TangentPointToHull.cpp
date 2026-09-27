// get_tangent(C, p): C = hull() output (strictly convex, CCW), p strictly outside.
// Contract (from the cyc_tsearch doc): a = gao(1) has ori(p, C[x], C[a]) <= 0 for all x,
// b = gao(-1) has ori(p, C[x], C[b]) >= 0 for all x. Checked against every vertex.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "9_Else/cyc_tsearch.cpp"
#include "8_Geometry/Convex_hull.cpp"
#include "8_Geometry/TangentPointToHull.cpp"

bool strictlyOutside(const vector<pll> &C, pll p) {
  int n = SZ(C);
  if (n <= 2) return !btw(C[0], C.back(), p);
  FOR (i, 0, n - 1) if (ori(C[i], C[(i + 1) % n], p) < 0) return true;
  return false;
}
int main() {
  int cases = 0, onEdgeLine = 0, small = 0;
  FOR (it, 1, 60000) {
    int R = it % 3 ? 5 : 1000, n = rnd(3, it % 4 ? 12 : 60);
    vector<pll> C(n);
    for (auto &x : C) x = pll(rnd(-R, R), rnd(-R, R));
    if (it % 10 == 0) for (auto &x : C) x.Y = 3 * x.X - 2; // collinear -> hull of size 2
    if (it % 50 == 0) C.resize(1);
    pll c0 = C[0];
    hull(C);
    if (C.empty()) C = {c0}; // hull() of one point is empty
    small += SZ(C) < 3;
    rotate(C.begin(), C.begin() + rnd(0, SZ(C) - 1), C.end());
    FOR (k, 1, 10) {
      pll p(rnd(-R - 3, R + 3), rnd(-R - 3, R + 3));
      if (!strictlyOutside(C, p)) continue;
      pii r = get_tangent(C, p);
      auto [a, b] = r;
      assert(0 <= a && a < SZ(C) && 0 <= b && b < SZ(C));
      FOR (x, 0, SZ(C) - 1) {
        assert(ori(p, C[x], C[a]) <= 0);
        assert(ori(p, C[x], C[b]) >= 0);
        onEdgeLine += ori(p, C[x], C[a]) == 0 && x != a;
      }
      assert(ori(p, C[a], C[b]) >= 0 && (a != b || SZ(C) == 1 || ori(p, C[0], C.back()) == 0));
      cases++;
    }
  }
  printf("TangentPointToHull: %d queries OK (%d with p collinear to a hull edge, %d hulls of size 1-2)\n", cases, onEdgeLine, small);
}
