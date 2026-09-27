#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/Intersection_of_line_and_circle.cpp"
// circleLine(c, r, a, b): points of line ab (a != b) on circle (c, r).
double lineDist(pdd a, pdd b, pdd p) { return fabs(cross(b - a, p - a)) / abs(b - a); }
int main() {
  int cnt = 0, tan0 = 0, tan1 = 0, tan2 = 0;
  FOR (it, 1, 300000) {
    bool integ = it % 2;
    int L = it % 4 < 2 ? 5 : 50;
    pdd a, b, c; double r, r2 = 0;
    if (integ) {
      a = pdd(rnd(-L, L), rnd(-L, L)), b = pdd(rnd(-L, L), rnd(-L, L)), c = pdd(rnd(-L, L), rnd(-L, L));
      r2 = rnd(0, 2 * L * L), r = sqrt(r2);
    } else {
      a = pdd(rndd(-L, L), rndd(-L, L)), b = pdd(rndd(-L, L), rndd(-L, L)), c = pdd(rndd(-L, L), rndd(-L, L));
      r = rndd(0, 2 * L);
    }
    if (a == b) continue;
    cnt++;
    auto v = circleLine(c, r, a, b);
    assert(SZ(v) <= 2);
    for (pdd p : v) {
      assert(fabs(abs(p - c) - r) < 1e-6);
      assert(lineDist(a, b, p) < 1e-6);
    }
    double d = lineDist(a, b, c);
    if (integ) { // exact classification: cross^2 vs r^2 |b-a|^2
      ll s = (ll)cross(b - a, c - a), l2 = (ll)abs2(b - a);
      __int128 lhs = (__int128)s * s, rhs = (__int128)r2 * l2;
      if (lhs < rhs) assert(SZ(v) == 2 && abs(v[0] - v[1]) > 1e-9);
      else if (lhs > rhs) assert(v.empty());
      else { (SZ(v) == 0 ? tan0 : SZ(v) == 1 ? tan1 : tan2)++; if (SZ(v) == 2) assert(abs(v[0] - v[1]) < 1e-6); }
    } else {
      if (d < r - 1e-6) assert(SZ(v) == 2);
      if (d > r + 1e-6) assert(v.empty());
    }
    if (SZ(v) == 2) assert(sign(dot(v[1] - v[0], b - a)) >= 0); // ordered along a->b
  }
  printf("circleLine: %d cases OK; exact-tangent int cases -> 0/1/2 pts: %d/%d/%d\n", cnt, tan0, tan1, tan2);
}
