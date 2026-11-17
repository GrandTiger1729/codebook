// manhattan_mst + Kruskal vs O(n^2) Prim on the complete graph; also checks every
// returned edge has the right weight and that there are at most 4n of them.
#include "prelude.h"
#include "stress.h"
#include "2_Graph/ManhattanMST.cpp"
int main() {
  int cases = 0;
  FOR (it, 1, 20000) {
    int n = rnd(1, it % 10 ? 12 : 200);
    ll R = it % 3 ? 5 : 1000000000;
    vector<pair<ll, ll>> p(n);
    for (auto &[x, y] : p) x = rnd(-R, R), y = rnd(-R, R);
    auto d = [&](int i, int j) { return abs(p[i].F - p[j].F) + abs(p[i].S - p[j].S); };
    vector<ll> best(n, LLONG_MAX); vector<int> in(n, 0);
    ll want = 0; best[0] = 0;
    FOR (r, 1, n) {
      int u = -1;
      FOR (i, 0, n - 1) if (!in[i] && (u < 0 || best[i] < best[u])) u = i;
      in[u] = 1, want += best[u];
      FOR (i, 0, n - 1) if (!in[i]) best[i] = min(best[i], d(u, i));
    }
    auto e = manhattan_mst(p);
    assert((int)e.size() <= 4 * n);
    for (auto [w, i, j] : e) assert(w == d(i, j));
    sort(e.begin(), e.end());
    vector<int> dsu(n); iota(dsu.begin(), dsu.end(), 0);
    function<int(int)> f = [&](int x) { return dsu[x] == x ? x : dsu[x] = f(dsu[x]); };
    ll got = 0; int comps = n;
    for (auto [w, i, j] : e) if (f(i) != f(j)) dsu[f(i)] = f(j), got += w, comps--;
    assert(comps == 1 && got == want);
    cases++;
  }
  printf("ManhattanMST ok: %d point sets (n<=200, coordinates up to 1e9, many duplicates)\n", cases);
}
