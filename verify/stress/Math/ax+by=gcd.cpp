#include "prelude.h"
#include "stress.h"
#include "6_Math/ax+by=gcd.cpp"
typedef __int128 L;

// the recipe in the comment: minimum non-negative x of ax+by=res
pll minx(ll a, ll b, ll res) {
  ll g = gcd(a, b);
  pll p = exgcd(a, b);
  p.F *= res / g, p.S *= res / g;
  ll t;
  if (p.F < 0) t = (abs(p.F) + b / g - 1) / (b / g);
  else t = -(p.F / (b / g));
  p.F += b / g * t, p.S += -a / g * t;
  return p;
}
int main() {
  int cases = 0;
  FOR (it, 1, 2000000) {
    ll a, b;
    int k = it % 4;
    if (k == 0) a = rnd(-30, 30), b = rnd(-30, 30);
    else if (k == 1) a = rnd(0, 1e9), b = rnd(0, 1e9);
    else if (k == 2) { ll g = rnd(1, 1e6); a = g * rnd(0, 1e12), b = g * rnd(0, 1e12); }
    else a = rnd(-(ll)1e18, (ll)1e18), b = rnd(-(ll)1e18, (ll)1e18);
    auto [x, y] = exgcd(a, b);
    ll g = gcd(a, b);
    L s = (L)a * x + (L)b * y;
    assert(s == g || s == -g);
    // bounded coefficients (so a*x never overflows in normal use)
    if (g) assert(abs(x) <= max<ll>(1, abs(b) / g) && abs(y) <= max<ll>(1, abs(a) / g));
    cases++;
    if (k == 1 && a > 0 && b > 0) { // exercise the comment's recipe on small-ish values
      ll res = g * rnd(0, 1000);
      auto [px, py] = minx(a, b, res);
      assert((L)a * px + (L)b * py == res);
      assert(0 <= px && px < b / g);
    }
  }
  // for a,b >= 0 the returned sign is always +g
  FOR (a, 0, 200) FOR (b, 0, 200) {
    auto [x, y] = exgcd(a, b);
    assert(a * x + b * y == gcd(a, b)), cases++;
  }
  printf("exgcd: %d cases OK\n", cases);
}
