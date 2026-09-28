#include "prelude.h"
#include "stress.h"
ll fpow(ll b, ll e, ll m) { // as a contestant would write it
  ll r = 1 % m; b %= m;
  for (; e; e >>= 1, b = b * b % m) if (e & 1) r = r * b % m;
  return r;
}
#include "6_Math/DiscreteLog.cpp"

int main() {
  ll cases = 0, none = 0;
  // exhaustive: every m <= 120, 0 <= x, y < m. x^k mod m has pre-period <= log2(m)
  // and period <= m, so the first m + 100 powers contain every reachable value.
  FOR (m, 1, 120) FOR (x, 0, m - 1) {
    vector<int> first(m, -1);
    ll s = 1 % m;
    for (int k = 0; k <= m + 100; k++, s = s * x % m) if (first[s] == -1) first[s] = k;
    FOR (y, 0, m - 1) {
      assert(DiscreteLog(x, y, m) == first[y]);
      cases++, none += first[y] == -1;
    }
  }
  assert(none > 0);
  // larger m: answers past the 100-step prefix; plant y = x^k and check minimality
  FOR (it, 1, 2000) {
    int m = it <= 100 ? rnd(2, 1e9) : rnd(2, 1e6), x = rnd(0, m - 1);
    int k = rnd(0, 1) ? rnd(0, 300) : rnd(0, 1e6);
    int y = fpow(x, k, m);
    int r = DiscreteLog(x, y, m);
    assert(0 <= r && r <= k && fpow(x, r, m) == y);
    if (r < 100000) { // brute minimality
      ll s = 1 % m;
      for (int j = 0; j < r; j++, s = s * x % m) assert(s != y);
    }
    cases++;
  }
  printf("DiscreteLog: %lld cases OK (%lld with no solution)\n", cases, none);
}
