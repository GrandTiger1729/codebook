#include "../prelude.h"
// the 8_Geometry chapter prelude supplies these; the driver is outside it
typedef pair<ll, ll> pll;
#define X first
#define Y second
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
// Convex_hull works on pll, but 8_Geometry/Default_code.cpp only defines the
// double (pdd) primitives, so the integer ones come from the driver.
typedef __int128 lll;
int sign(lll a) { return a == 0 ? 0 : a > 0 ? 1 : -1; }
lll cross(pll a, pll b) { return (lll)a.X * b.Y - (lll)a.Y * b.X; }
int ori(pll a, pll b, pll c) {
  return sign((lll)(b.X - a.X) * (c.Y - a.Y) - (lll)(b.Y - a.Y) * (c.X - a.X));
}
#include "../../codebook/8_Geometry/Convex_hull.cpp"

int main() {
  Waimai;
  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    vector<pll> d(n);
    for (auto &[x, y] : d) cin >> x >> y;
    sort(ALL(d)); d.erase(unique(ALL(d)), d.end());
    if (d.size() <= 2u) {
      cout << d.size() << '\n';
      for (auto [x, y] : d) cout << x << ' ' << y << '\n';
      continue;
    }
    hull(d);
    cout << d.size() << '\n';
    for (auto [x, y] : d) cout << x << ' ' << y << '\n';
  }
}
