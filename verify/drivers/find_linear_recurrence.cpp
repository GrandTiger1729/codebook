#include "../prelude.h"
const ll MOD = 998244353;
ll pw(ll a, ll b) { ll r = 1; for (a %= MOD; b; b >>= 1, a = a * a % MOD) if (b & 1) r = r * a % MOD; return r; }
struct mint {
  ll v = 0;
  mint(ll x = 0) : v(((x % MOD) + MOD) % MOD) {}
  mint operator-() const { return mint(-v); }
  mint &operator+=(mint o) { v = (v + o.v) % MOD; return *this; }
  mint &operator-=(mint o) { v = (v - o.v + MOD) % MOD; return *this; }
  mint operator*(mint o) const { return mint(v * o.v); }
  mint operator/(mint o) const { return mint(v * pw(o.v, MOD - 2)); }
  bool operator==(int x) const { return v == x; }
};
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"

int main() {
  Waimai;
  int n; cin >> n;
  vector<mint> a(n);
  for (auto &x : a) { ll v; cin >> v; x = mint(v); }
  auto c = BerlekampMassey(a);  // 0-based
  int d = (int)c.size();
  cout << d << '\n';
  for (int i = 0; i < d; i++) cout << c[i].v << " \n"[i == d - 1];
  if (!d) cout << '\n';
}
