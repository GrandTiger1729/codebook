#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/Intersection_of_two_circles.cpp"
// CCinter(a, b, p1, p2): 1 iff circles touch/cross; p1 = left of a.O->b.O, p2 = right.
int main() {
  int cnt = 0, conc = 0, tanMiss = 0, tanHit = 0;
  FOR (it, 1, 400000) {
    bool integ = it % 2;
    int L = it % 4 < 2 ? 4 : 40;
    Cir a, b; ll d2i = 0;
    if (integ) {
      a.O = pdd(rnd(-L, L), rnd(-L, L)), b.O = pdd(rnd(-L, L), rnd(-L, L));
      a.R = rnd(0, L), b.R = rnd(0, L);
      d2i = (ll)abs2(a.O - b.O);
    } else {
      a.O = pdd(rndd(-L, L), rndd(-L, L)), b.O = pdd(rndd(-L, L), rndd(-L, L));
      a.R = rndd(0, L), b.R = rndd(0, L);
    }
    if (a.O == b.O) { conc++; continue; } // concentric: see note (a.R == b.R gives NaN)
    cnt++;
    pdd p1(1e18, 1e18), p2 = p1;
    bool ret = CCinter(a, b, p1, p2);
    double d = abs(a.O - b.O);
    if (ret) {
      for (pdd p : {p1, p2}) {
        assert(fabs(abs(p - a.O) - a.R) < 1e-6);
        assert(fabs(abs(p - b.O) - b.R) < 1e-6);
      }
      assert(ori(a.O, b.O, p1) >= 0 && ori(a.O, b.O, p2) <= 0);
    }
    if (integ) { // exact: (R1-R2)^2 <= d2 <= (R1+R2)^2
      ll s = (ll)(a.R + b.R), df = (ll)(a.R - b.R);
      bool touch = df * df <= d2i && d2i <= s * s;
      bool tangent = df * df == d2i || d2i == s * s;
      if (!tangent) assert(ret == touch);
      else (ret ? tanHit : tanMiss)++;
      if (touch && !tangent) assert(abs(p1 - p2) > 1e-9);
    } else {
      if (d < a.R + b.R - 1e-7 && d > fabs(a.R - b.R) + 1e-7) assert(ret && abs(p1 - p2) > 1e-9);
      if (d > a.R + b.R + 1e-7 || d < fabs(a.R - b.R) - 1e-7) assert(!ret);
    }
  }
  // concentric circles: document behaviour
  Cir a{pdd(0, 0), 1}, b{pdd(0, 0), 1}; pdd p1, p2;
  bool r = CCinter(a, b, p1, p2);
  printf("CCinter: %d cases OK; exact int tangencies hit/miss %d/%d; identical circles -> ret=%d p1=(%g,%g)\n",
         cnt, tanHit, tanMiss, r, p1.X, p1.Y);
}
