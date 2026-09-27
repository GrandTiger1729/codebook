#include "../prelude.h"
// the 8_Geometry chapter prelude supplies these; the driver is outside it
typedef pair<ll, ll> pll;
#define X first
#define Y second
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
typedef __int128 lll;
int sign(lll a) { return a == 0 ? 0 : a > 0 ? 1 : -1; }
lll cross(pll a, pll b) { return (lll)a.X * b.Y - (lll)a.Y * b.X; }
lll abs2(pll a) { return (lll)a.X * a.X + (lll)a.Y * a.Y; }
#include "../../codebook/8_Geometry/Polar_Angle_Sort.cpp"

int main() {
  Waimai;
  int n; cin >> n;
  vector<pll> p(n);
  for (auto &[x, y] : p) cin >> x >> y;
  sort(ALL(p), [](pll a, pll b) { return cmp(a, b); });
  // The template starts the order at the +x axis; LC wants ascending atan2 in
  // (-pi, pi], i.e. starting just above -pi with the -x axis last. Rotate.
  int k = 0;
  while (k < n && !(p[k].Y < 0 || (p[k].Y == 0 && p[k].X < 0))) k++;   // first of the "neg" half
  int j = k;
  while (j < n && p[j].Y == 0 && p[j].X < 0) j++;                      // skip the -x axis run
  vector<pll> r;
  for (int i = j; i < n; i++) r.pb(p[i]);
  for (int i = 0; i < k; i++) r.pb(p[i]);
  for (int i = k; i < j; i++) r.pb(p[i]);
  for (auto [x, y] : r) cout << x << ' ' << y << '\n';
}
