#include "../prelude.h"
#include "../../codebook/6_Math/Miller_Rabin.cpp"
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
