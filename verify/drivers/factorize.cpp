#include "../prelude.h"
#include "../../codebook/6_Math/Miller_Rabin.cpp"
// Pollard_Rho calls prime(n); Miller_Rabin.cpp exports Miller_Rabin(a, n)
// and the base list only as a comment, so the driver still has to bridge them.
bool prime(ll n) {
  if (n < 2) return false;
  for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
#include "../../codebook/6_Math/Pollard_Rho.cpp"

int main() {
  Waimai;
  int q; cin >> q;
  while (q--) {
    ll a; cin >> a;
    cnt.clear();
    PollardRho(a);
    int k = 0;
    for (auto &[p, e] : cnt) k += e;
    cout << k;
    for (auto &[p, e] : cnt) for (int i = 0; i < e; i++) cout << ' ' << p;
    cout << '\n';
  }
}
