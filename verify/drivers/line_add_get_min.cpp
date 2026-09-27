#include "../prelude.h"
#include "../../codebook/9_Else/DynamicConvexTrick.cpp"

int main() {
  Waimai;
  int n, q; cin >> n >> q;
  DynamicHull h;  // maintains max, so negate both the line and the answer
  for (int i = 0; i < n; i++) { ll a, b; cin >> a >> b; h.addline(-a, -b); }
  while (q--) {
    int t; cin >> t;
    if (t == 0) { ll a, b; cin >> a >> b; h.addline(-a, -b); }
    else { ll p; cin >> p; cout << -h.query(p) << '\n'; }
  }
}
