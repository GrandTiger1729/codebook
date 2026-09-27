#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
// projection / reflection / linearTransformation vs complex<long double>
typedef complex<long double> C;
C c(pdd p) { return C(p.X, p.Y); }
bool near(pdd a, C b, long double s) { return abs(c(a) - b) <= 1e-9 * max((long double)1, s); }
pdd rp(int k) { return k ? pdd(rnd(-5, 5), rnd(-5, 5)) : pdd(rndd(-1e3, 1e3), rndd(-1e3, 1e3)); }
int main() {
  int cnt = 0;
  FOR (it, 1, 300000) {
    int k = it & 1;
    pdd a = rp(k), b = rp(k), p = rp(k), q0 = rp(k), q1 = rp(k), r = rp(k);
    if (abs(a - b) < 1e-3) continue;
    long double s = abs(c(a)) + abs(c(b)) + abs(c(p)) + abs(c(q0)) + abs(c(q1)) + abs(c(r));
    C d = c(b) - c(a), u = d / abs(d);
    C pr = c(a) + u * real((c(p) - c(a)) * conj(u));        // foot of perpendicular
    assert(near(projection(a, b, p), pr, s));
    assert(near(reflection(a, b, p), 2.l * pr - c(p), s));
    // similarity z -> q0 + (z - p0) * (q1 - q0) / (p1 - p0)
    C lt = c(q0) + (c(r) - c(a)) * (c(q1) - c(q0)) / d;
    long double sc = s * (1 + abs(c(q1) - c(q0)) / abs(d));
    assert(near(linearTransformation(a, b, q0, q1, r), lt, sc));
    assert(near(linearTransformation(a, b, q0, q1, a), c(q0), sc));
    assert(near(linearTransformation(a, b, q0, q1, b), c(q1), sc));
    cnt++;
  }
  printf("Default_code: %d cases OK (projection/reflection/linearTransformation, ints +-5 and doubles +-1e3)\n", cnt);
}
