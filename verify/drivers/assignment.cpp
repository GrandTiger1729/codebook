#include "../prelude.h"
const int N = 505;
const ll INF = 1e18;
#include "../../codebook/4_Flow_Matching/Kuhn_Munkres.cpp"

// KM now takes init(n, m) and starts weights at 0 (i.e. it maximises weight
// without forcing a perfect matching, and add_edge keeps the max). LC needs a
// perfect matching minimising cost, so shift every weight to be non-negative:
// with w >= 0 on a complete n*n graph the maximum-weight matching is perfect.
const ll SHIFT = 1000000000;
KM km;

int main() {
  Waimai;
  int n; cin >> n;
  km.init(n, n);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) { ll a; cin >> a; km.add_edge(i, j, -a + SHIFT); }
  ll best = km.solve();
  for (int i = 0; i < n; i++) assert(km.fl[i] != -1);   // must be perfect
  cout << (ll)n * SHIFT - best << '\n';
  for (int i = 0; i < n; i++) cout << km.fl[i] << " \n"[i + 1 == n];
}
