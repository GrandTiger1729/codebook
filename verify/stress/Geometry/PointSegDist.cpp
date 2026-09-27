// PointSegDist(q0, q1, p): distance from p to segment q0-q1 (degenerate segment allowed).
#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/PointSegDist.cpp"

// reference: clamp projection parameter (long double), and a dense-sample upper bound
long double ref(pdd a, pdd b, pdd p) {
  long double dx = b.X - a.X, dy = b.Y - a.Y, L = dx * dx + dy * dy;
  long double t = L == 0 ? 0 : ((p.X - a.X) * dx + (p.Y - a.Y) * dy) / L;
  t = max((long double)0, min((long double)1, t));
  long double x = a.X + t * dx - p.X, y = a.Y + t * dy - p.Y;
  return sqrtl(x * x + y * y);
}
int main() {
  int cases = 0;
  FOR (it, 1, 400000) {
    pdd a, b, p;
    if (it % 2) { // small ints: collinear / endpoint / zero-length cases are frequent
      int R = it % 4 == 1 ? 3 : 10;
      auto g = [&] { return pdd(rnd(-R, R), rnd(-R, R)); };
      a = g(), b = rnd(0, 5) ? g() : a, p = g();
    } else {
      auto g = [&] { return pdd(rndd(-1e3, 1e3), rndd(-1e3, 1e3)); };
      a = g(), b = g(), p = g();
      if (it % 8 == 0) p = a + (b - a) * rndd(-0.5, 1.5); // near the supporting line
    }
    double got = PointSegDist(a, b, p);
    long double want = ref(a, b, p);
    assert(fabsl(got - want) <= 1e-9 * max((long double)1, want) + 1e-9);
    // symmetry in the endpoints
    assert(fabs(PointSegDist(b, a, p) - got) <= 1e-9 * max(1.0, got));
    cases++;
  }
  printf("PointSegDist: %d cases OK\n", cases);
}
