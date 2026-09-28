#include "prelude.h"
#include "stress.h"
#include "6_Math/ModMin.cpp"

// min k >= 0 with l <= (a*k) mod m <= r, or -1. (a*k) mod m has period m in k.
ll brute(ll a, ll m, ll l, ll r) {
  for (ll k = 0; k < m; k++) if (l <= a * k % m && a * k % m <= r) return k;
  return -1;
}
int main() {
  ll cases = 0, none = 0;
  // exhaustive: every m <= 60, 0 <= a < m, 0 <= l <= r < m
  FOR (m, 1, 60) FOR (a, 0, m - 1) FOR (l, 0, m - 1) FOR (r, l, m - 1) {
    ll want = brute(a, m, l, r);
    assert(mod_min(a, m, l, r) == want);
    cases++, none += want == -1;
  }
  assert(none > 0); // the -1 branch was exercised
  // random, m up to 1e5 (brute is O(m))
  FOR (it, 1, 3000) {
    ll m = rnd(1, 100000), a = rnd(0, m - 1), l = rnd(0, m - 1), r = rnd(l, min(m - 1, l + rnd(0, 50)));
    assert(mod_min(a, m, l, r) == brute(a, m, l, r)), cases++;
  }
  // large m (brute infeasible): -1 iff no multiple of gcd(a, m) lies in [l, r];
  // otherwise the answer is feasible and none of the 200 k's below it are
  FOR (it, 1, 20000) {
    ll m = rnd(1, 1e9), a = rnd(0, m - 1), l = rnd(0, m - 1), r = rnd(l, m - 1);
    if (it & 1) { // big gcd, narrow [l, r]: hits -1 often
      ll g = rnd(1, 1e5); m = g * rnd(1, 1e4), a = g * rnd(0, m / g - 1);
      l = rnd(0, m - 1), r = min(m - 1, l + rnd(0, 2 * g));
    }
    ll k = mod_min(a, m, l, r);
    ll g = gcd(a, m); // {a*k mod m} = multiples of g in [0, m)
    assert((k == -1) == (r / g * g < l));
    if (k == -1) { cases++; continue; }
    assert(0 <= k && k < m);
    assert(l <= a * k % m && a * k % m <= r);
    for (ll j = max(0LL, k - 200); j < k; j++) assert(!(l <= a * j % m && a * j % m <= r));
    cases++;
  }
  printf("ModMin: %lld cases OK (%lld with no solution)\n", cases, none);
}
