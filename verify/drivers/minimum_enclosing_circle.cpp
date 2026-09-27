#include "../prelude.h"
#include "../geo_prelude.h"
#include "../../codebook/8_Geometry/Heart.cpp"
#include "../../codebook/8_Geometry/Minimum_Enclosing_Circle.cpp"

int main() {
  Waimai;
  int n; cin >> n;
  vector<pdd> p(n);
  for (auto &q : p) cin >> q.X >> q.Y;
  double r;
  pdd c = Minimum_Enclosing_Circle(p, r);
  // |x|,|y| <= 1e4, so 1e-6 is far above the double noise and far below the
  // gap between "on the circle" and "strictly inside" for integer inputs.
  for (auto &q : p) cout << (abs(q - c) > r - 1e-6 ? '1' : '0');
  cout << '\n';
}
