#include "../prelude.h"
typedef complex<ll> cpx;
#include "../../codebook/6_Math/Gaussian_gcd.cpp"

int main() {
  Waimai;
  int t; cin >> t;
  while (t--) {
    ll a1, b1, a2, b2; cin >> a1 >> b1 >> a2 >> b2;
    cpx a(a1, b1), b(a2, b2);
    if (a == cpx(0, 0) && b == cpx(0, 0)) { cout << "0 0\n"; continue; }
    if (b == cpx(0, 0)) swap(a, b);
    cpx g = gaussian_gcd(a, b);
    cout << g.real() << ' ' << g.imag() << '\n';
  }
}
