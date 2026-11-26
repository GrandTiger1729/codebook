// josephus(n, k) vs simulation (n <= 300, k <= 400); vs the O(n) recurrence (n <= 1e7,
// k up to 1e18 with n <= 1e5); vs the k = 2 closed form (n <= 1e18); and for n up to 1e18
// with k <= 1000 vs an iterative forward version of the recurrence that skips wrap-free steps.
#include "prelude.h"
#include "stress.h"
#include "6_Math/Josephus.cpp"
ll rec(ll n, ll k) { // J(i) = (J(i - 1) + k) mod i
  ll s = 0;
  for (ll i = 2; i <= n; i++) s = (s + k % i) % i;
  return s;
}
// Same recurrence, forward from m = 1: while s + k < m + 1 the mod never fires, so t steps
// at once give s += t k, m += t, valid while s + t (k - 1) <= m - 1. O(k log n) iterations.
ll jump(ll n, ll k) {
  if (k == 1) return n - 1;
  ll s = 0, m = 1; // s = J(m)
  while (m < n) {
    ll t = min(n - m, (m - 1 - s) / (k - 1));
    if (t == 0) s = (s + k) % (m + 1), m++;
    else s += t * k, m += t;
  }
  return s;
}
ll closed2(ll n) { return 2 * (n - (1LL << __lg(n))); }
int main() {
  int cases = 0;
  FOR (n, 1, 300) FOR (k, 1, 400) {
    if (k > 60 && k % 37) continue;
    vector<int> a(n); iota(a.begin(), a.end(), 0);
    int pos = 0;
    while (a.size() > 1) pos = (pos + k - 1) % a.size(), a.erase(a.begin() + pos);
    assert(josephus(n, k) == a[0]); cases++;
  }
  FOR (it, 1, 300) {
    ll n = rnd(1, 100000), k = rnd(1, it % 2 ? 50 : 100000), s = 0;
    FOR (i, 2, n) s = (s + k) % i;
    assert(josephus(n, k) == s); cases++;
  }
  FOR (it, 1, 300) { // moderate n, any k: two references. k > n recurses n deep, so keep
    // n <= 1e5 there (template limit: k > n overflows an 8 MB stack near n = 2.6e5 at -O2)
    ll k = it % 3 ? rnd(1, 3000) : rnd(1, (ll)1e18), n = rnd(1, it % 10 || k > 3000 ? 100000 : 10000000);
    ll want = rec(n, k);
    if (k <= 3000) assert(jump(n, k) == want);
    assert(josephus(n, k) == want); cases++;
  }
  FOR (it, 1, 100000) { // k = 2 closed form up to 1e18
    ll n = it <= 64 ? (1LL << (it - 1) % 63) - it / 32 % 2 : rnd(1, (ll)1e18);
    if (n < 1) n = 1;
    assert(closed2(n) == jump(n, 2));
    assert(josephus(n, 2) == closed2(n)); cases++;
  }
  FOR (it, 1, 2000) { // n up to 1e18, small k (template recursion depth ~ k ln(n / k) + k)
    ll n = it % 4 ? rnd(1, (ll)1e18) : rnd(1, 1000000), k = rnd(1, it % 10 ? 30 : 1000);
    if (it % 50 == 0) n = (ll)1e18;
    assert(josephus(n, k) == jump(n, k)); cases++;
  }
  printf("Josephus ok: %d cases (n up to 1e18)\n", cases);
}
