#include "prelude.h"
#include "stress.h"
#include "6_Math/Miller_Rabin.cpp"
#include "6_Math/Pollard_Rho.cpp"
// cnt must equal the true factorization, and must accumulate across calls
map<ll, int> trial(ll n) {
  map<ll, int> r;
  for (ll p = 2; p * p <= n; p++) while (n % p == 0) r[p]++, n /= p;
  if (n > 1) r[n]++;
  return r;
}
void check(ll n) {
  cnt.clear(), PollardRho(n);
  ll prod = 1;
  for (auto [p, e] : cnt) { assert(prime(p)); FOR (i, 1, e) prod *= p; }
  assert(prod == n);
}
int main() {
  int c = 0;
  FOR (n, 1, 200000) { cnt.clear(), PollardRho(n); assert(cnt == trial(n)); c++; }
  const ll P = 1000000007, Q = 998244353, R = 4294967291LL;
  for (ll base : {1LL, 3LL, P, Q, R, P * Q, 3LL * 5 * 7 * 11 * 13})
    for (ll k = 0; (base << k) > 0 && (base << k) / base == (1LL << k); k++)
      check(base << k), c++;             // many factors of 2 in front
  FOR (it, 1, 20000) {
    ll n = rnd(1, (ll)1e18); check(n), c++;
    ll m = rnd(1, 1LL << 40) << rnd(0, 20);
    if (m > 0) check(m), c++;
  }
  auto nextPrime = [](ll x) { while (!prime(x)) x++; return x; };
  FOR (it, 1, 300) {                      // p*q, p^2, p^3 with large primes; Carmichael numbers
    ll p = nextPrime(rnd(2, 1e9)), q = nextPrime(rnd(2, 1e9)), r = nextPrime(rnd(2, 1e6));
    check(p * q), check(p * p), check(r * r * r), check(r * p), c += 4;
  }
  for (ll n : {561LL, 41041LL, 825265LL, 321197185LL, 5394826801LL, 232250619601LL, 9746347772161LL})
    check(n), c++;
  map<ll, int> acc; cnt.clear();          // two calls accumulate into cnt
  PollardRho(12), PollardRho(1LL << 40);
  assert(cnt[2] == 42 && cnt[3] == 1 && cnt.size() == 2);
  printf("Pollard_Rho: %d cases OK (n<=2e5 exhaustive vs trial div, 2^k*p up to 2^62, random 1e18, large semiprimes and prime powers, Carmichael)\n", c);
}
