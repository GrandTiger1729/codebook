// inside(prv, cur, nxt, p, strict): does ray cur->p start into the interior of a CCW polygon
// at vertex cur (interior angle = CCW sweep from cur->nxt to cur->prv)? strict = 1 excludes the
// two boundary rays, strict = 0 includes them. Reference: exact CCW angle comparison.
// Needs prv, nxt, p != cur. prv, nxt in the same direction (0/360 degree spike) is ambiguous
// and skipped; prv, nxt opposite (180 degrees) is tested.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "8_Geometry/Vector_in_poly.cpp"

// Reference in __int128 (dot / cross of |coord| <= 2e9 vectors are under test in the template).
typedef __int128 L;
L dot128(pll a, pll b) { return (L)a.X * b.X + (L)a.Y * b.Y; }
L cross128(pll a, pll b) { return (L)a.X * b.Y - (L)a.Y * b.X; }
// half of w measured CCW from base: 0 for angle in [0, pi), 1 for [pi, 2pi)
int half(pll base, pll w) {
  L c = cross128(base, w);
  return c < 0 || (c == 0 && dot128(base, w) < 0);
}
// CCW angle of v measured from base, in [0, 2pi); compare two such angles
bool angLess(pll base, pll u, pll v) { // ang(u) < ang(v)
  int hu = half(base, u), hv = half(base, v);
  if (hu != hv) return hu < hv;
  return cross128(u, v) > 0; // same half: angle from u to v is in (-pi, pi)
}
bool angEq(pll base, pll u, pll v) { return !angLess(base, u, v) && !angLess(base, v, u); }
int main() {
  int cases = 0, reflex = 0, straight = 0, onb = 0, big = 0;
  const ll M = 1e9; // contest size: |coord| <= 1e9
  auto inBox = [&](pll q) { return llabs(q.X) <= M && llabs(q.Y) <= M; };
  FOR (it, 1, 2000000) {
    ll R = it <= 1000000 ? (it % 3 ? 3 : 50) : M;
    auto g = [&] { return pll(rnd(-R, R), rnd(-R, R)); };
    pll prv = g(), cur = g(), nxt = g(), p = g();
    if (R == M) { // large: also rays along / opposite to / just beside the two edges
      auto along = [&](pll from, pll to, ll sgn) { // lattice point on ray from->to (or its opposite)
        pll d = to - from;
        ll gg = __gcd(llabs(d.X), llabs(d.Y));
        if (!gg) return to;
        d = d / gg * sgn;
        ll lim = M / max(llabs(d.X), llabs(d.Y)); // stays in the box
        pll q = from + d * rnd(1, max(1LL, lim / 2));
        return inBox(q) ? q : to;
      };
      int t = rnd(0, 7);
      if (t == 1) p = along(cur, nxt, 1);
      if (t == 2) p = along(cur, prv, 1);
      if (t == 3) prv = along(cur, nxt, -1); // straight angle
      if (t == 4) p = along(cur, nxt, -1);
      if (t == 5) { p = along(cur, rnd(0, 1) ? nxt : prv, 1) + pll(rnd(-1, 1), rnd(-1, 1)); if (!inBox(p)) continue; }
      if (t == 6) prv = along(cur, nxt, 1) + pll(rnd(-1, 1), rnd(-1, 1)); // almost a spike
      if (!inBox(prv)) continue;
    }
    if (prv == cur || nxt == cur || p == cur) continue;
    pll d1 = nxt - cur, d2 = prv - cur, v = p - cur;
    if (cross128(d1, d2) == 0 && dot128(d1, d2) > 0) continue; // spike: ambiguous
    straight += cross128(d1, d2) == 0, reflex += cross128(d1, d2) < 0;
    // interior: 0 < ang(v) < ang(d2), measured CCW from d1
    bool onB = angEq(d1, v, d1) || angEq(d1, v, d2);
    bool in = angLess(d1, v, d2) && !angEq(d1, v, d1);
    onb += onB;
    FOR (st, 0, 1) {
      bool want = st ? in : (in || onB);
      assert(inside(prv, cur, nxt, p, st) == want);
    }
    cases++, big += R == M;
  }
  printf("Vector_in_poly: %d cases OK (%d with |coord|<=1e9; %d reflex, %d straight, %d on a boundary ray)\n", cases, big, reflex, straight, onb);
}
