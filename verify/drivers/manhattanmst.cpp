#include "../prelude.h"
#include "../../codebook/2_Graph/ManhattanMST.cpp"

int dsu[200005];
int find(int x) { return dsu[x] == x ? x : dsu[x] = find(dsu[x]); }

int main() {
  Waimai;
  int n; cin >> n;
  vector<pair<ll, ll>> p(n);
  for (auto &[x, y] : p) cin >> x >> y;
  auto e = manhattan_mst(p);
  sort(e.begin(), e.end());
  iota(dsu, dsu + n, 0);
  ll tot = 0; vector<pair<int, int>> used;
  for (auto [w, i, j] : e)
    if (find(i) != find(j)) dsu[find(i)] = find(j), tot += w, used.pb(i, j);
  cout << tot << '\n';
  for (auto [i, j] : used) cout << i << ' ' << j << '\n';
}
