#include "../prelude.h"
// The snippet is written with int; N goes up to 1e12 here, so widen int for it.
#define int ll
vector<ll> quotients(ll n) {
  vector<ll> res;
  // body of 6_Math/floor_enumeration.cpp, with `res.pb(x)` added to observe it
  for (int l = 1, r; l <= n; l = r + 1) {
    int x = n / l;
    r = n / x;
    res.pb(x);
  }
  return res;
}
#undef int

int main() {
  Waimai;
  ll n; cin >> n;
  auto v = quotients(n);
  sort(v.begin(), v.end());
  cout << v.size() << '\n';
  for (size_t i = 0; i < v.size(); i++) cout << v[i] << " \n"[i + 1 == v.size()];
}
