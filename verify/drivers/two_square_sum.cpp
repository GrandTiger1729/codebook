#include "../prelude.h"
#include "../../codebook/6_Math/Miller_Rabin.cpp"
bool prime(ll n) {
  if (n < 2) return false;
  for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
#include "../../codebook/6_Math/Pollard_Rho.cpp"
#include "../../codebook/6_Math/Gaussian_gcd.cpp"

// Follows the Sum of squares theory bullet: n = a^2 + b^2 needs every
// p = 3 (mod 4) to an even power; a prime p = 1 (mod 4) is N(gcd(p, c + i))
// with c^2 = -1; every a + bi is a unit times a product of those factors.
mt19937_64 rng(20261126);
ll pw(ll a, ll e, ll m) {
  ll r = 1;
  for (a %= m; e; e >>= 1, a = (__int128)a * a % m)
    if (e & 1) r = (__int128)r * a % m;
  return r;
}
cpx split(ll p) {  // pi with N(pi) = p, p = 1 (mod 4)
  ll c;
  do c = pw(2 + rng() % (p - 3), (p - 1) / 4, p);
  while ((__int128)c * c % p != p - 1);
  return gaussian_gcd(cpx(p, 0), cpx(c, 1));
}

int main() {
  Waimai;
  int q; cin >> q;
  while (q--) {
    ll n; cin >> n;
    if (n == 0) { cout << "1\n0 0\n"; continue; }
    cnt.clear();
    PollardRho(n);
    vector<cpx> zs{cpx(1, 0)};
    bool ok = true;
    for (auto [p, e] : cnt) {
      if (p % 4 == 3) {
        if (e % 2) { ok = false; break; }
        for (auto &z : zs) FOR(i, 1, e / 2) z *= cpx(p, 0);
      } else if (p == 2) {
        for (auto &z : zs) FOR(i, 1, e) z *= cpx(1, 1);
      } else {
        cpx pi = split(p), pib = conj(pi);
        vector<cpx> nz;
        for (auto z : zs) FOR(k, 0, e) {
          cpx w = z;
          FOR(i, 1, k) w *= pi;
          FOR(i, 1, e - k) w *= pib;
          nz.pb(w);
        }
        zs = nz;
      }
    }
    if (!ok) { cout << "0\n"; continue; }
    vector<pair<ll, ll>> out;
    for (auto z : zs) {
      while (!(z.real() > 0 && z.imag() >= 0)) z *= cpx(0, 1);
      ll a = z.real(), b = z.imag();
      assert((__int128)a * a + (__int128)b * b == n);
      out.pb(a, b);
      if (b == 0) out.pb(0, a);  // (0, a) is the rotation not in a > 0
    }
    cout << out.size() << '\n';
    for (auto [a, b] : out) cout << a << ' ' << b << '\n';
  }
}
