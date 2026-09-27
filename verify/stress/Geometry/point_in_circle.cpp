// in_cc(p, q): q strictly inside circumcircle of triangle p (integer). Reference: exact
// circumcenter as a rational (Dx/D, Dy/D), compare squared distances in __int128.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp" // integer chapter prelude (pll abs2/cross/operator-)
#include "stress.h"
#include "8_Geometry/point_in_circle.cpp"

typedef __int128 L;
// returns sign(R^2 - |q-O|^2): 1 inside, 0 on, -1 outside. Needs non-collinear p.
int brute(array<pll, 3> p, pll q) {
  pll b = p[1] - p[0], c = p[2] - p[0], r = q - p[0];
  L D = 2 * (L)cross(b, c), bb = abs2(b), cc = abs2(c);
  L ox = c.Y * bb - b.Y * cc, oy = b.X * cc - c.X * bb; // O - p0 = (ox, oy) / D
  L R2 = ox * ox + oy * oy;
  L dx = r.X * D - ox, dy = r.Y * D - oy, d2 = dx * dx + dy * dy;
  return R2 > d2 ? 1 : R2 == d2 ? 0 : -1;
}
int main() {
  int cases = 0, on = 0, cw = 0, big = 0;
  FOR (it, 1, 400000) {
    ll R = it % 3 == 0 ? 1000000 : it % 3 == 1 ? 3 : 8; // 1e6: det terms ~1e12*1e12, still < 2^127
    array<pll, 3> p;
    FOR (i, 0, 2) p[i] = pll(rnd(-R, R), rnd(-R, R));
    int o = ori(p[0], p[1], p[2]);
    if (!o) continue;
    pll q = rnd(0, 4) ? pll(rnd(-R, R), rnd(-R, R)) : p[rnd(0, 2)];
    int want = brute(p, q);
    on += want == 0, big += R > 100;
    if (o > 0) assert(in_cc(p, q) == (want > 0));
    else {     // CW triangle: the determinant flips sign, so in_cc means "strictly outside"
      assert(in_cc(p, q) == (want < 0));
      cw++;
    }
    cases++;
  }
  printf("point_in_circle: %d cases OK (%d on-circle, %d CW, %d with |coord|<=1e6)\n", cases, on, cw, big);
}
