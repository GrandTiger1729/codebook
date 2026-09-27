#include "../prelude.h"
ll fpow(ll a, ll n, ll m) {
  ll r = 1 % m;
  for (a %= m; n; n >>= 1, a = a * a % m) if (n & 1) r = r * a % m;
  return r;
}
#include "../../codebook/6_Math/DiscreteLog.cpp"

int main() {
  Waimai;
  int t; cin >> t;
  while (t--) {
    int x, y, m; cin >> x >> y >> m;
    cout << DiscreteLog(x, y, m) << '\n';
  }
}
