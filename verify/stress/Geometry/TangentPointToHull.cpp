// get_tangent(C, p): C = hull() output (strictly convex, CCW), p outside or on the hull.
// Contract: a = gao(1) has ori(p, C[x], C[a]) <= 0 for all x, b = gao(-1) has
// ori(p, C[x], C[b]) >= 0 for all x. Checked against every vertex. For p on the hull
// (hull of >= 3 points): p inside edge (i, i+1) gives exactly (a, b) = (i+1, i);
// p == C[i] gives a in {i, i+1}, b in {i-1, i}.
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
  int cases = 0, onEdgeLine = 0, small = 0, onVertex = 0, onEdge = 0;
  FOR (it, 1, 60000) {
    ll R = it % 3 ? 5 : it % 6 ? 1000 : 1000000000;
    int n = rnd(3, it % 4 ? 12 : 60);
    vector<pll> C(n);
    for (auto &x : C) x = pll(rnd(-R, R), rnd(-R, R));
    if (it % 10 == 0) for (auto &x : C) x.Y = R > 1000 ? -x.X : 3 * x.X - 2; // collinear -> hull of size 2
    if (it % 50 == 0) C.resize(1);
    pll c0 = C[0];
    hull(C);
    if (C.empty()) C = {c0}; // hull() of one point is empty
    int m = SZ(C);
    small += m < 3;
    rotate(C.begin(), C.begin() + rnd(0, m - 1), C.end());
    FOR (k, 1, 12) {
      pll p(rnd(-R - 3, R + 3), rnd(-R - 3, R + 3));
      int vert = -1, edge = -1; // p == C[vert] / p strictly inside edge (edge, edge+1)
      if (k % 3 == 0) p = C[vert = rnd(0, m - 1)];
      if (k % 3 == 1 && m >= 2) { // a lattice point strictly inside an edge, if any
        int i = rnd(0, m - 1), j = (i + 1) % m;
        pll e = C[j] - C[i];
        ll g = __gcd(abs(e.X), abs(e.Y));
        if (g < 2) continue;
        p = C[i] + e / g * rnd(1, g - 1), edge = i;
      }
      if (vert < 0 && edge < 0 && !strictlyOutside(C, p)) continue;
      auto [a, b] = get_tangent(C, p);
      assert(0 <= a && a < m && 0 <= b && b < m);
      FOR (x, 0, m - 1) {
        assert(ori(p, C[x], C[a]) <= 0);
        assert(ori(p, C[x], C[b]) >= 0);
        onEdgeLine += ori(p, C[x], C[a]) == 0 && x != a && C[x] != p && C[a] != p;
      }
      assert(ori(p, C[a], C[b]) >= 0);
      if (m >= 3 && edge >= 0) assert(a == (edge + 1) % m && b == edge), onEdge++;
      if (m >= 3 && vert >= 0) {
        assert(a == vert || a == (vert + 1) % m);
        assert(b == vert || b == (vert + m - 1) % m);
        onVertex++;
      }
      cases++;
    }
  }
  printf("TangentPointToHull: %d queries OK (%d on a vertex, %d inside an edge, %d collinear ties, %d hulls of size 1-2)\n",
         cases, onVertex, onEdge, onEdgeLine, small);
}
