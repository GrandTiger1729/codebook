// inside(prv, cur, nxt, p, strict): does ray cur->p start into the interior of a CCW polygon
// at vertex cur (interior angle = CCW sweep from cur->nxt to cur->prv)? strict = 1 excludes the
// two boundary rays, strict = 0 includes them. Reference: exact CCW angle comparison.
// Needs prv, nxt, p != cur. prv, nxt in the same direction (0/360 degree spike) is ambiguous
// and skipped; prv, nxt opposite (180 degrees) is tested.
#include "prelude.h"
#include "8_Geometry/Default_code_int.cpp"
#include "stress.h"
#include "8_Geometry/Vector_in_poly.cpp"

int half(pll v) { return v.Y < 0 || (v.Y == 0 && v.X < 0); }
// CCW angle of v measured from base, in [0, 2pi); compare two such angles
bool angLess(pll base, pll u, pll v) { // ang(u) < ang(v)
  auto key = [&](pll w) { // rotate so that base is along +x: (dot, cross)
    return pll(dot(base, w), cross(base, w));
  };
  pll a = key(u), b = key(v);
  if (half(a) != half(b)) return half(a) < half(b);
  return cross(a, b) > 0;
}
bool angEq(pll base, pll u, pll v) { return !angLess(base, u, v) && !angLess(base, v, u); }
int main() {
  int cases = 0, reflex = 0, straight = 0, onb = 0;
  FOR (it, 1, 1000000) {
    int R = it % 3 ? 3 : 50;
    auto g = [&] { return pll(rnd(-R, R), rnd(-R, R)); };
    pll prv = g(), cur = g(), nxt = g(), p = g();
    if (prv == cur || nxt == cur || p == cur) continue;
    pll d1 = nxt - cur, d2 = prv - cur, v = p - cur;
    if (cross(d1, d2) == 0 && dot(d1, d2) > 0) continue; // spike: ambiguous
    straight += cross(d1, d2) == 0, reflex += cross(d1, d2) < 0;
    // interior: 0 < ang(v) < ang(d2), measured CCW from d1
    bool onB = angEq(d1, v, d1) || angEq(d1, v, d2);
    bool in = angLess(d1, v, d2) && !angEq(d1, v, d1);
    onb += onB;
    FOR (st, 0, 1) {
      bool want = st ? in : (in || onB);
      assert(inside(prv, cur, nxt, p, st) == want);
    }
    cases++;
  }
  printf("Vector_in_poly: %d cases OK (%d reflex, %d straight, %d on a boundary ray)\n", cases, reflex, straight, onb);
}
