#include "../prelude.h"
#include "../../codebook/9_Else/BinarySearchOnFraction.cpp"

// pred is declared inside the template and defined here.
ll TX, TY;   // compare against TX/TY
bool pred(Q q) { return (__int128)q.p * TY >= (__int128)q.q * TX; }

int main() {
  Waimai;
  int t; cin >> t;
  while (t--) {
    ll n, x, y; cin >> n >> x >> y;
    // smallest fraction >= x/y
    TX = x, TY = y;
    Q up = frac_bs(n);
    // largest fraction <= x/y: the same search on the reciprocal, then flip
    TX = y, TY = x;
    Q rl = frac_bs(n);
    Q lo{rl.q, rl.p};
    cout << lo.p << ' ' << lo.q << ' ' << up.p << ' ' << up.q << '\n';
  }
}
