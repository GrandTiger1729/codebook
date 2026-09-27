#include "../prelude.h"
// min_heap is now in content.tex; DSU still is not (memorised instead)
#include "../../codebook/3_Data_Structure/DSU.cpp"
#include "../../codebook/3_Data_Structure/min_heap.cpp"
#include "../../codebook/2_Graph/Minimum_Arborescence_fast.cpp"

int main() {
  Waimai;
  int n, m, s; cin >> n >> m >> s;
  vector<E> e(m);
  for (auto &[a, b, c] : e) cin >> a >> b >> c;
  auto ids = dmst(e, n, s);
  vector<int> p(n, s);
  ll tot = 0;
  for (int id : ids) p[e[id].t] = e[id].s, tot += e[id].w;
  cout << tot << '\n';
  for (int i = 0; i < n; i++) cout << p[i] << " \n"[i + 1 == n];
}
