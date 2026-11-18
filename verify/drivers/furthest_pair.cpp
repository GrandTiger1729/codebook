#include "../prelude.h"
#include "../../codebook/8_Geometry/Default_code_int.cpp"
#include "../../codebook/8_Geometry/Convex_hull.cpp"
#include "../../codebook/8_Geometry/FarthestPair.cpp"
int main() {
  Waimai;
  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    vector<pll> p(n);
    for (auto &[x, y] : p) cin >> x >> y;
    map<pll, int> id;
    FOR (i, 0, n - 1) id.emplace(p[i], i);
    vector<pll> C = p;
    sort(ALL(C)), C.erase(unique(ALL(C)), C.end()); // hull() keeps duplicates of a lone point
    if (SZ(C) > 1) hull(C);
    if (SZ(C) < 2) { cout << "0 1\n"; continue; }
    auto [a, b] = farthest_pair(C);
    cout << id[C[a]] << ' ' << id[C[b]] << '\n';
  }
}
