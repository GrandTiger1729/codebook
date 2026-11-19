#include "prelude.h"
#include "stress.h"
#include "6_Math/exgcd.cpp"
typedef __int128 L;

int main() {
  int cases = 0;
  FOR (it, 1, 2000000) {
    ll a, b;
    int k = it % 4;
    if (k == 0) a = rnd(-30, 30), b = rnd(-30, 30);
    else if (k == 1) a = rnd(0, 1e9), b = rnd(0, 1e9);
    else if (k == 2) { ll g = rnd(1, 1e6); a = g * rnd(0, 1e12), b = g * rnd(0, 1e12); }
    else a = rnd(-(ll)1e18, (ll)1e18), b = rnd(-(ll)1e18, (ll)1e18);
    ll x, y, r = exgcd(a, b, x, y);
    ll g = gcd(a, b);
    assert(r == g || r == -g);
    assert((L)a * x + (L)b * y == r);
    // bounded coefficients (so a*x never overflows in normal use)
    if (g) assert(abs(x) <= max<ll>(1, abs(b) / g) && abs(y) <= max<ll>(1, abs(a) / g));
    cases++;
    if (a >= 0 && b > 0) assert(abs(x) <= b / g); // the documented bound (CRT relies on it)
  }
  // for a,b >= 0 the returned sign is always +g
  FOR (a, 0, 200) FOR (b, 0, 200) {
    ll x, y, r = exgcd(a, b, x, y);
    assert(r == gcd(a, b) && a * x + b * y == r), cases++;
  }
  printf("exgcd: %d cases OK\n", cases);
}
