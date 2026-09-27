#include "prelude.h"
#include "stress.h"
#include "2_Graph/Vizing.cpp"

int main() {
  int cases = 0;
  for (int it = 0; it < 4000; it++) {
    int n = it % 4 ? rnd(1, 12) : rnd(13, 104);
    int dens = rnd(0, 100);
    vector<pair<int, int>> E;
    FOR (i, 1, n) FOR (j, i + 1, n) if (rnd(0, 99) < dens) rnd(0, 1) ? E.pb(i, j) : E.pb(j, i);
    shuffle(E.begin(), E.end(), rng);
    vector<int> deg(n + 1);
    for (auto [u, v] : E) deg[u]++, deg[v]++;
    int D = *max_element(deg.begin(), deg.end());
    vizing::init(n);
    vizing::solve(E);
    for (auto [u, v] : E) {
      int c = vizing::G[u][v];
      assert(c == vizing::G[v][u] && 1 <= c && c <= D + 1);
    }
    FOR (u, 1, n) {
      vector<int> seen(D + 2);
      FOR (v, 1, n) if (int c = vizing::G[u][v]) assert(!seen[c]++);
      int cnt = 0;
      FOR (v, 1, n) cnt += !!vizing::G[u][v];
      assert(cnt == deg[u]); // no phantom edges
    }
    cases++;
  }
  printf("%d graphs, n<=104, proper & <= Delta+1 colours\n", cases);
}
