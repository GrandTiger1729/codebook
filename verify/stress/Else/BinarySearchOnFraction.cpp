#include "prelude.h"
#include "stress.h"
#include "9_Else/BinarySearchOnFraction.cpp"
ll X, Y, calls;
bool pred(Q a) { calls++; return (__int128)a.p * Y >= (__int128)X * a.q; } // p/q >= X/Y

int main() {
  int cases = 0;
  // brute force: every p, q <= N, for every target X/Y with X, Y <= 12
  FOR (N, 1, 30) FOR (x, 0, 12) FOR (y, 1, 12) {
    X = x, Y = y;
    Q best{1, 0};
    FOR (p, 0, N) FOR (q, 1, N) if (pred({p, q}) && p * best.q < best.p * q) best = {p, q};
    Q got = frac_bs(N);
    assert(got.p * best.q == best.p * got.q && got.p <= N && got.q <= N && pred(got));
    cases++;
  }
  // large: answer is the closest fraction above; check pred and that no smaller fraction fits
  // (Farey neighbour test against the largest fraction below, found on the reciprocal)
  ll most = 0;
  FOR (it, 1, 100000) {
    ll N = it % 2 ? rnd(1, 1e18) : rnd(1, 1e9), y = rnd(1, N), x = it % 5 ? rnd(0, N) : 1;
    X = x, Y = y, calls = 0;
    Q up = frac_bs(N);
    most = max(most, calls);
    X = y, Y = x ? x : 1;
    if (!x) { assert(up.p == 0 && up.q == 1); cases++; continue; }
    Q rl = frac_bs(N), lo{rl.q, rl.p};
    X = x, Y = y;
    assert(up.p <= N && up.q <= N && pred(up));
    // lo <= x/y <= up, and lo, up adjacent in Farey sequence F_N (or equal)
    __int128 det = (__int128)up.p * lo.q - (__int128)lo.p * up.q;
    assert(det == 0 || det == 1);
    cases++;
  }
  printf("BinarySearchOnFraction: %d cases OK (max %lld pred calls at N <= 1e18)\n", cases, most);
}
