// XorBasis vs brute force: max xor of a subset of a[l..r] via one basis copy per prefix
// (insert a[i] at time i, query max_xor(l) on copy r), max_xor with a start value x,
// and kth over the whole span (sorted distinct subset xors, -1 past the end).
#include "prelude.h"
#include "stress.h"
#include "6_Math/XorBasis.cpp"
int main() {
  int cases = 0;
  FOR (it, 1, 3000) {
    int n = rnd(1, 9), bits = it % 3 ? 4 : 62;
    vector<ll> a(n);
    for (ll &x : a) x = rnd(0, (1LL << bits) - 1) & (it % 5 ? -1LL : 0b1011);
    vector<XorBasis> pre(n);
    XorBasis cur;
    FOR (i, 0, n - 1) cur.insert(a[i], i), pre[i] = cur;
    FOR (l, 0, n - 1) FOR (r, l, n - 1) {
      ll x0 = rnd(0, (1LL << bits) - 1), best = 0, bestx = 0;
      FOR (S, 0, (1 << (r - l + 1)) - 1) {
        ll v = 0; FOR (i, 0, r - l) if (S >> i & 1) v ^= a[l + i];
        best = max(best, v), bestx = max(bestx, v ^ x0);
      }
      assert(pre[r].max_xor(l) == best && pre[r].max_xor(l, x0) == bestx);
      cases++;
    }
    set<ll> span;
    FOR (S, 0, (1 << n) - 1) { ll v = 0; FOR (i, 0, n - 1) if (S >> i & 1) v ^= a[i]; span.insert(v); }
    ll k = 0;
    for (ll v : span) assert(cur.kth(k++) == v);
    assert(cur.kth(k) == -1 && cur.kth(k + 5) == -1);
  }
  printf("XorBasis ok: %d range queries, kth over the full span\n", cases);
}
