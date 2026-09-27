#include "../prelude.h"
#include "../../codebook/4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp"

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  Matching g(n);
  while (m--) { int a, b; cin >> a >> b; g.add_edge(a, b); }
  cout << g.solve() << '\n';
  for (int i = 0; i < n; i++)
    if (g.match[i] != n && i < g.match[i]) cout << i << ' ' << g.match[i] << '\n';
}
