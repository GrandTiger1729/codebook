// in_cc(p, q): q strictly inside circumcircle of triangle p (integer). Reference: exact
// circumcenter as a rational (Dx/D, Dy/D), compare squared distances in __int128 (|coord| <=
// 1e6); and, up to |coord| <= 1e9, the lifted determinant relative to q in 256-bit (split).
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
// sign of det |x_i y_i x_i^2 + y_i^2| over rows p_i - q (> 0: q inside the CCW circle).
// z_i * c_i reaches 8e18 * 8e18 at |coord| = 1e9 (sum > 2^127), so z_i is split into
// 32-bit halves: det = H * 2^32 + Lo with both partial sums well inside __int128.
int lifted(array<pll, 3> p, pll q) {
  pll d[3];
  FOR (i, 0, 2) d[i] = pll(p[i].X - q.X, p[i].Y - q.Y);
  L H = 0, Lo = 0;
  FOR (i, 0, 2) {
    pll a = d[(i + 1) % 3], b = d[(i + 2) % 3];
    L c = (L)a.X * b.Y - (L)a.Y * b.X, z = (L)d[i].X * d[i].X + (L)d[i].Y * d[i].Y;
    H += (z >> 32) * c, Lo += (z & 0xffffffff) * c;
  }
  L carry = Lo >= 0 ? Lo >> 32 : -((-Lo + 0xffffffff) >> 32); // floor(Lo / 2^32)
  H += carry, Lo -= carry << 32;                              // now 0 <= Lo < 2^32
  return H ? (H > 0 ? 1 : -1) : (Lo > 0);
}
int main() {
  int cases = 0, on = 0, cw = 0, big = 0, huge = 0;
  auto test = [&](array<pll, 3> p, pll q, bool small) {
    int o = ori(p[0], p[1], p[2]);
    if (!o) return false;
    int want = lifted(p, q) * o; // as if p were CCW
    if (small) assert(want == brute(p, q));
    on += want == 0;
    if (o > 0) assert(in_cc(p, q) == (want > 0));
    else {     // CW triangle: the determinant flips sign, so in_cc means "strictly outside"
      assert(in_cc(p, q) == (want < 0));
      cw++;
    }
    return ++cases, true;
  };
  FOR (it, 1, 400000) {
    ll R = it % 3 == 0 ? 1000000 : it % 3 == 1 ? 3 : 8; // circumcentre brute fits __int128 up to 1e6
    array<pll, 3> p;
    FOR (i, 0, 2) p[i] = pll(rnd(-R, R), rnd(-R, R));
    pll q = rnd(0, 4) ? pll(rnd(-R, R), rnd(-R, R)) : p[rnd(0, 2)];
    big += test(p, q, true) && R > 100;
  }
  const ll M = 1e9; // contest size; also exactly cocircular points: c + the 8 symmetries of (x, y)
  FOR (it, 1, 400000) {
    array<pll, 3> p;
    pll q;
    if (it % 2) {
      FOR (i, 0, 2) p[i] = pll(rnd(-M, M), rnd(-M, M));
      q = rnd(0, 4) ? pll(rnd(-M, M), rnd(-M, M)) : p[rnd(0, 2)];
    } else {
      ll r = rnd(1, it % 4 ? M / 2 : 3), x = rnd(0, r), y = rnd(0, r), cx = rnd(-M + r, M - r), cy = rnd(-M + r, M - r);
      pll s[8] = {{x, y}, {-y, x}, {-x, -y}, {y, -x}, {x, -y}, {y, x}, {-x, y}, {-y, -x}};
      FOR (i, 0, 2) p[i] = pll(cx, cy) + s[rnd(0, 7)];
      q = pll(cx, cy) + s[rnd(0, 7)] + pll(rnd(-1, 1), rnd(-1, 1)) * rnd(0, 1);
      if (llabs(q.X) > M || llabs(q.Y) > M) continue;
    }
    huge += test(p, q, false);
  }
  printf("point_in_circle: %d cases OK (%d on-circle, %d CW, %d with |coord|<=1e6, %d with |coord|<=1e9)\n", cases, on, cw, big, huge);
}
