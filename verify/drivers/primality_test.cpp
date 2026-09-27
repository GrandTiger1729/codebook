#include "../prelude.h"
#include "../../codebook/6_Math/Miller_Rabin.cpp"

int main() {
  Waimai;
  int q; cin >> q;
  while (q--) {
    ll n; cin >> n;
    bool ok = n >= 2;
    if (ok) for (ll a : {2LL, 325LL, 9375LL, 28178LL, 450775LL, 9780504LL, 1795265022LL})
      if (!Miller_Rabin(a, n)) { ok = false; break; }
    cout << (ok ? "Yes" : "No") << '\n';
  }
}
