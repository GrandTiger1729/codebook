// farthest_pair on hull() of random point sets (with collinear points and duplicates
// removed) vs O(n^2) over the hull vertices, coordinates up to 1e9.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "8_Geometry/Convex_hull.cpp"
#include "8_Geometry/FarthestPair.cpp"
int main() {
  int cases = 0;
  FOR (it, 1, 30000) {
    int n = rnd(2, it % 10 ? 15 : 300);
    ll R = it % 3 ? 5 : 1000000000;
    vector<pll> p(n);
    for (auto &q : p) q = pll(rnd(-R, R), rnd(-R, R));
    if (it % 7 == 0) for (auto &q : p) q.Y = q.X; // collinear
    sort(ALL(p)), p.erase(unique(ALL(p)), p.end());
    if (SZ(p) < 2) continue;
    hull(p);
    ll best = 0;
    for (auto a : p) for (auto b : p) best = max(best, abs2(a - b));
    auto [i, j] = farthest_pair(p);
    assert(abs2(p[i] - p[j]) == best);
    cases++;
  }
  printf("FarthestPair ok: %d hulls\n", cases);
}
