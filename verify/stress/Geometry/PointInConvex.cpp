// PointInConvex(C, p, strict): C = output of hull() (strictly convex, CCW, may be size 1/2),
// also tested reversed (CW) and rotated. Reference: O(n) orientation test.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "8_Geometry/Convex_hull.cpp"
#include "8_Geometry/PointInConvex.cpp"

bool brute(const vector<pll> &C, pll p, bool strict) {
  int n = SZ(C);
  if (n == 1) return !strict && C[0] == p;
  if (n == 2) return !strict && btw(C[0], C[1], p);
  int s = ori(C[0], C[1], C[2]); // orientation of the polygon
  FOR (i, 0, n - 1) {
    int o = ori(C[i], C[(i + 1) % n], p) * s;
    if (o < 0 || (strict && o == 0)) return false;
  }
  return true;
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
  printf("PointInConvex: %d queries OK (hull sizes 1/2/>=3: %d/%d/%d)\n", cases, sz[1], sz[2], sz[3]);
}
