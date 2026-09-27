// Minkowski(A, B) vs hull() of all |A||B| pairwise sums (exact vector equality: both are
// hull() output = strictly convex, CCW, starting at the lexicographically smallest point).
// Precondition "|A|,|B| >= 3" is read as: hull(A), hull(B) have >= 3 points; the collinear
// (hull size 2) case is probed separately and reported, not asserted.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "8_Geometry/Convex_hull.cpp"
#include "8_Geometry/Minkowski_Sum.cpp"

vector<pll> brute(const vector<pll> &A, const vector<pll> &B) {
  vector<pll> C;
  for (auto a : A) for (auto b : B) C.pb(a + b);
  hull(C);
  return C;
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
  printf("Minkowski_Sum: %d cases OK (collinear input, not asserted: %d match, %d differ)\n", cases, degOK, degBad);
}
