#include "prelude.h"
#include "stress.h"
const int MAXP = 1e6 + 5;
ll mpow(ll a, ll e, ll m) { // memorised helper the template expects
  ll r = 1 % m; a %= m;
  for (; e; e >>= 1, a = a * a % m) if (e & 1) r = r * a % m;
  return r;
}
#include "6_Math/fac_no_p.cpp"

int main() {
  int cases = 0;
  vector<pair<ll, ll>> pp; // (p, p^k)
  for (ll p : {2, 3, 5, 7, 11, 13, 97, 997, 1009})
    for (ll pk = p; pk <= 1000000; pk *= p) pp.pb(p, pk);
  for (auto [p, pk] : pp) {
    int big = pk > 10000; // each call is O(pk): sample sparsely
    // direct: running product of n! with every factor p stripped, mod pk
    int LIM = pk <= 1000 ? 30000 : 3000;
    ll cur = 1;
    FOR (n, 0, LIM) {
      if (n) { ll v = n; while (v % p == 0) v /= p; cur = cur * (v % pk) % pk; }
      if (big ? rnd(0, 150) == 0 : (n < 200 || rnd(0, 30) == 0)) assert(fac_no_p(n, p, pk) == cur), cases++;
    }
    // large n (up to 1e18): f(n+1) = f(n) * (n+1 with factors p stripped)
    FOR (it, 1, big ? 10 : 200) {
      ll n = rnd(1, (ll)1e18 - 1);
      ll v = n + 1; while (v % p == 0) v /= p;
      assert(fac_no_p(n + 1, p, pk) == fac_no_p(n, p, pk) * (v % pk) % pk), cases++;
    }
  }
  printf("fac_no_p: %d cases OK\n", cases);
}
