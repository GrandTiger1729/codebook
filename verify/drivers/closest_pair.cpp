#include "../prelude.h"
#include "../geo_prelude.h"
#include "../../codebook/8_Geometry/ClosestPair.cpp"

int main() {
  Waimai;
  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    vector<pll> v(n);
    map<pll, vector<int>> id;   // LC wants the indices, not the points
    for (int i = 0; i < n; i++) {
      cin >> v[i].X >> v[i].Y;
      id[v[i]].push_back(i);
    }
    auto [a, b] = closest_pair(v);
    int i = id[a].back(); id[a].pop_back();
    int j = id[b].back();
    cout << i << ' ' << j << '\n';
  }
}
