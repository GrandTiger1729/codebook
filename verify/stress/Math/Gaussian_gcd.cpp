#include "prelude.h"
#include "stress.h"
#include "6_Math/Gaussian_gcd.cpp"
#include "6_Math/Miller_Rabin.cpp"
// gaussian_gcd(a, b) (b != 0), cpx = complex<__int128_t>:
// 1. small |coords| <= 30: g divides a and b, and N(g) equals the largest
//    norm of a common divisor found by enumerating all d with |coords| <= 30;
// 2. big |coords| <= 1e18 built as a = h x, b = h y: g divides a, b and h | g;
// 3. Sum of squares: prime p = 1 (mod 4) up to 4e18, c = r^((p-1)/4) with
//    c^2 = -1, then N(gcd(p, c + i)) = p.
typedef __int128_t L;
L N(cpx z) { return z.real() * z.real() + z.imag() * z.imag(); }
bool dvd(cpx d, cpx a) {
  cpx q = a * conj(d);
  return q.real() % N(d) == 0 && q.imag() % N(d) == 0;
}
bool prime(ll n) {
  if (n < 2) return false;
  for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
ll pw(ll a, ll e, ll m) {
  ll r = 1;
  for (a %= m; e; e >>= 1, a = (L)a * a % m)
    if (e & 1) r = (L)r * a % m;
  return r;
}
int main() {
  int cases = 0;
  FOR (it, 1, 2000) {
    cpx a(rnd(-30, 30), rnd(-30, 30)), b(rnd(-30, 30), rnd(-30, 30));
    if (b == cpx(0, 0)) continue;
    cpx g = gaussian_gcd(a, b);
    assert(dvd(g, a) && dvd(g, b));
    L best = 0;
    FOR (x, -30, 30) FOR (y, -30, 30) if (x || y) {
      cpx d(x, y);
      if (dvd(d, a) && dvd(d, b)) best = max(best, N(d));
    }
    assert(N(g) == best);
    cases++;
  }
  FOR (it, 1, 20000) {
    ll H = it % 2 ? 1000 : 1000000000;
    cpx h(rnd(-H, H), rnd(-H, H));
    if (h == cpx(0, 0)) continue;
    ll R = 1000000000000000000LL / (H * 2);
    cpx x(rnd(-R, R), rnd(-R, R)), y(rnd(-R, R), rnd(-R, R));
    if (y == cpx(0, 0)) continue;
    cpx a = h * x, b = h * y, g = gaussian_gcd(a, b);
    assert(dvd(g, a) && dvd(g, b) && dvd(h, g));
    cases++;
  }
  for (ll lim : {1000LL, 2000000000LL, 4000000000000000000LL})
    FOR (it, 1, 2000) {
      ll p;
      do p = rnd(lim / 2, lim); while (p % 4 != 1 || !prime(p));
      ll c;
      do c = pw(rnd(2, p - 2), (p - 1) / 4, p);
      while ((L)c * c % p != p - 1);
      assert(N(gaussian_gcd(cpx(p, 0), cpx(c, 1))) == p);
      cases++;
    }
  printf("Gaussian_gcd ok: %d cases (sum of squares p up to 4e18)\n", cases);
}
