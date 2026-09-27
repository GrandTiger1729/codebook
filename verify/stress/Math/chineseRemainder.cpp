#include "prelude.h"
#include "stress.h"
#include "6_Math/ax+by=gcd.cpp" // exgcd, as in the book
#include "6_Math/chineseRemainder.cpp"
typedef __int128 L;

void check(ll x1, ll m1, ll x2, ll m2) {
  ll g = gcd(m1, m2), lcm = m1 / g * m2;
  ll r = solve(x1, m1, x2, m2);
  if ((x2 - x1) % g) { assert(r == -1); return; }
  assert(0 <= r && r < lcm);
  assert(r % m1 == x1 && r % m2 == x2);
}
int main() {
  int cases = 0;
  // exhaustive tiny
  FOR (m1, 1, 24) FOR (m2, 1, 24) FOR (x1, 0, m1 - 1) FOR (x2, 0, m2 - 1) {
    ll r = solve(x1, m1, x2, m2), best = -1;
    FOR (x, 0, m1 * m2 - 1) if (x % m1 == x1 && x % m2 == x2) { best = x; break; }
    assert(r == best), cases++;
  }
  // moduli up to 1e6 (lcm <= 1e12): intermediates p.F*(x2-x1)*m1 stay < 1e18
  FOR (it, 1, 300000) {
    ll g = rnd(1, 1000), m1 = g * rnd(1, 1000), m2 = g * rnd(1, 1000);
    ll x1 = rnd(0, m1 - 1), x2 = rnd(0, 1) ? rnd(0, m2 - 1) : (x1 + g * rnd(0, 1e6)) % m2;
    check(x1, m1, x2, m2), cases++;
  }
  // chained system with answer known in advance
  FOR (it, 1, 20000) {
    ll X = rnd(0, 1e11), M = 1, cur = 0;
    FOR (k, 1, 5) {
      ll m = rnd(1, 300);
      ll r = solve(cur, M, X % m, m);
      assert(r != -1);
      M = M / gcd(M, m) * m, cur = r;
      if (M > 1e11) break;
    }
    assert(cur == X % M), cases++;
  }
  // lcm near 1e18: the regime the "be careful with overflow" comment warns about
  FOR (it, 1, 20000) {
    ll m1 = rnd(5e8, 1e9), m2 = rnd(5e8, 1e9);
    ll x1 = rnd(0, m1 - 1), x2 = rnd(0, m2 - 1);
    if ((x2 - x1) % gcd(m1, m2)) continue;
    check(x1, m1, x2, m2), cases++;
  }
  printf("CRT: %d cases OK\n", cases);
}
