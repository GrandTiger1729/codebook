#include "prelude.h"
#include "stress.h"
#include "6_Math/QuadraticResidue.cpp"

int main() {
  ll cases = 0;
  // every a for every prime p < 2000: a root exactly when brute force finds one
  FOR (p, 2, 2000) {
    bool prime = true;
    for (int d = 2; d * d <= p; d++) prime &= p % d != 0;
    if (!prime) continue;
    vector<bool> sq(p);
    FOR (x, 0, p - 1) sq[1LL * x * x % p] = true;
    FOR (a, 0, p - 1) {
      int r = QuadraticResidue(a, p);
      if (!sq[a]) assert(r == -1);
      else assert(0 <= r && r < p && 1LL * r * r % p == a);
      cases++;
    }
  }
  // large primes: plant a = x^2
  for (int p : {998244353, 1000000007, 1000000009, 2147483647}) FOR (it, 1, 3000) {
    int x = rnd(0, p - 1), a = 1LL * x * x % p, r = QuadraticResidue(a, p);
    assert(0 <= r && r < p && 1LL * r * r % p == a);
    cases++;
  }
  printf("QuadraticResidue: %lld cases OK\n", cases);
}
