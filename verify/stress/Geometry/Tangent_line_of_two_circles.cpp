#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/Tangent_line_of_two_circles.cpp"
// go(c1, c2, 1): outer tangents, go(c1, c2, -1): inner tangents. Always 0 or 2 lines
// (the 2 coincide when the circles touch). Line.X is the touch point on c1.
double sdist(Line l, pdd p) { return cross(l.Y - l.X, p - l.X) / abs(l.Y - l.X); }
int main() {
  int cnt = 0, tanCnt[3] = {};
  FOR (it, 1, 300000) {
    bool integ = it % 2;
    int L = it % 4 < 2 ? 4 : 40;
    Cir c1, c2; ll d2i = 0;
    if (integ) {
      c1.O = pdd(rnd(-L, L), rnd(-L, L)), c2.O = pdd(rnd(-L, L), rnd(-L, L));
      c1.R = rnd(0, L), c2.R = rnd(0, L);
      d2i = (ll)abs2(c1.O - c2.O);
    } else {
      c1.O = pdd(rndd(-L, L), rndd(-L, L)), c2.O = pdd(rndd(-L, L), rndd(-L, L));
      c1.R = rndd(0, L), c2.R = rndd(0, L);
    }
    cnt++;
    double d = abs(c1.O - c2.O);
    for (int s1 : {1, -1}) {
      auto v = go(c1, c2, s1);
      assert(SZ(v) == 0 || SZ(v) == 2);
      for (Line l : v) {
        assert(abs(l.Y - l.X) > 1e-9);
        double e1 = sdist(l, c1.O), e2 = sdist(l, c2.O);
        assert(fabs(fabs(e1) - c1.R) < 1e-6 && fabs(fabs(e2) - c2.R) < 1e-6);
        if (c1.R > 1e-6 && c2.R > 1e-6) assert((e1 > 0) == (e2 > 0) ? s1 == 1 : s1 == -1);
        assert(fabs(abs(l.X - c1.O) - c1.R) < 1e-6); // X touches c1
      }
      if (SZ(v) == 2 && c1.R + c2.R > 1e-6) { // two different tangents unless degenerate
        double rr = s1 == 1 ? fabs(c1.R - c2.R) : c1.R + c2.R;
        if (d > rr + 1e-6) assert(fabs(sdist(v[0], v[1].X)) > 1e-9 || fabs(sdist(v[0], v[1].Y)) > 1e-9);
      }
      // expected count
      double rr = s1 == 1 ? c1.R - c2.R : c1.R + c2.R;
      if (integ) {
        ll r = (ll)rr;
        if (d2i == 0) assert(v.empty());
        else if (d2i > r * r) assert(SZ(v) == 2);
        else if (d2i < r * r) assert(v.empty());
        else tanCnt[SZ(v)]++;
      } else {
        if (d < 1e-9) assert(v.empty());
        else if (d > fabs(rr) + 1e-7) assert(SZ(v) == 2);
        else if (d < fabs(rr) - 1e-7) assert(v.empty());
      }
    }
  }
  printf("tangents: %d cases OK; exact int touching circles -> 0/2 lines: %d/%d\n", cnt, tanCnt[0], tanCnt[2]);
}
