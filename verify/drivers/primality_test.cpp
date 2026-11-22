#include "../prelude.h"
#include "../../codebook/6_Math/Miller_Rabin.cpp"

int main() {
  Waimai;
  int q; cin >> q;
  while (q--) {
    ll n; cin >> n;
    cout << (prime(n) ? "Yes" : "No") << '\n';
  }
}
