#include "prelude.h"
#include "stress.h"
// A contestant's modint. The template compares mints (x >= base), indexes a vector
// with one (poly[x - base]) and adds ints (base + i), so it needs an implicit int
// conversion plus int overloads of + and - to avoid ambiguity.
const int MOD = 998244353;
struct mint {
  int v;
  mint(ll x = 0) : v((x % MOD + MOD) % MOD) {}
  operator int() const { return v; }
  mint operator+(const mint &o) const { return mint(v + o.v); }
  mint operator-(const mint &o) const { return mint(v - o.v); }
  mint operator*(const mint &o) const { return mint(1ll * v * o.v); }
  mint operator+(int o) const { return mint((ll)v + o); }
  mint operator-(int o) const { return mint((ll)v - o); }
  mint &operator+=(const mint &o) { return *this = *this + o; }
};
mint mpw(mint a, ll e) { mint r = 1; for (; e; e >>= 1, a = a * a) if (e & 1) r = r * a; return r; }
const int K = 200;
mint ifac[K], inegfac[K]; // 1/i!, 1/(-1)^i i!
#include "7_Polynomial/Value_Poly.cpp"

mint eval(const vector<mint> &c, mint x) { mint r = 0; for (int i = (int)c.size() - 1; i >= 0; i--) r = r * x + c[i]; return r; }
int main() {
  mint f = 1;
  FOR (i, 0, K - 1) {
    if (i) f = f * mint(i);
    ifac[i] = mpw(f, MOD - 2);
    inegfac[i] = i & 1 ? mint(0) - ifac[i] : ifac[i];
  }
  int cases = 0;
  FOR (it, 1, 3000) {
    int d = rnd(0, 12);
    vector<mint> c(d + 1);
    for (auto &x : c) x = rnd(0, MOD - 1);
    if (it % 10 == 0) for (auto &x : c) x = 0; // zero polynomial
    ll base = rnd(0, 1) ? rnd(0, 20) : rnd(0, MOD - 100);
    Poly P(base, eval(c, base));
    FOR (i, 1, d) P.poly.pb(eval(c, base + i)); // f(base..base+d)
    FOR (q, 1, 20) { // arbitrary points, inside and outside the stored window
      ll x = rnd(0, 2) ? rnd(0, MOD - 1) : base + rnd(0, d);
      assert(P.get_val(x) == eval(c, x)), cases++;
    }
    // raise(): g(x) = sum_{t=base}^{x} f(t); do it a few times
    int R = rnd(1, 4);
    vector<vector<mint>> tab(R + 1, vector<mint>(d + R + 40)); // tab[r][k] = value at base+k
    FOR (k, 0, d + R + 39) tab[0][k] = eval(c, base + k);
    FOR (r, 1, R) FOR (k, 0, d + R + 39) tab[r][k] = (k ? tab[r][k - 1] : mint(0)) + tab[r - 1][k];
    FOR (r, 1, R) {
      P.raise();
      FOR (k, 0, d + R + 39) assert(P.get_val(base + k) == tab[r][k]), cases++;
    }
  }
  printf("Value_Poly: %d cases OK\n", cases);
}
