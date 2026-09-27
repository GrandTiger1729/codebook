#include "../prelude.h"
#include "../../codebook/6_Math/ModMin.cpp"

// mod_min answers "smallest k with (a*k mod m) in [l, r]"; LC asks for
// min over x < N of (A x + B) mod M, so binary search the answer on top of it.
ll A, M, Nn;
bool feasible(ll v, ll B) {
  ll L = (M - B % M) % M, R = L + v, best = -1;
  auto take = [&](ll lo, ll hi) {
    ll k = mod_min(A, M, lo, hi);
    if (k < 0) return;  // no solution in this window
    if (best < 0 || k < best) best = k;
  };
  if (R < M) take(L, R);
  else take(L, M - 1), take(0, R - M);
  return best >= 0 && best < Nn;
}

int main() {
  Waimai;
  int t; cin >> t;
  while (t--) {
    ll B; cin >> Nn >> M >> A >> B;
    ll lo = 0, hi = B % M;
    while (lo < hi) { ll mid = (lo + hi) / 2; if (feasible(mid, B)) hi = mid; else lo = mid + 1; }
    cout << lo << '\n';
  }
}
