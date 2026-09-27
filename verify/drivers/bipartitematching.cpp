#include "../prelude.h"
const int N = 100005;
#include "../../codebook/4_Flow_Matching/Bipartite_Matching.cpp"

Bipartite_Matching bm;

int main() {
  Waimai;
  int l, r, m; cin >> l >> r >> m;
  bm.init(l, r);
  while (m--) { int a, b; cin >> a >> b; bm.add_edge(a, b); }
  cout << bm.matching() << '\n';
  for (int i = 0; i < l; i++)
    if (~bm.mp[i]) cout << i << ' ' << bm.mp[i] << '\n';
}
