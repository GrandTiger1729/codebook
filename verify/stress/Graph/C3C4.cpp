#include "prelude.h"
#include "stress.h"
const int N = 200005;
#include "2_Graph/C3C4.cpp"
// C3 must report every triangle exactly once (as a set of three vertices)
// and C4 must equal the number of 4-cycles, both against O(n^3)/O(n^4)
// brute force on random simple graphs. The last part times a star plus a
// clique, where a wrong edge orientation would be quadratic.
int main() {
  int cases = 0;
  ll tri = 0, quad = 0;
  FOR (it, 1, 30000) {
    int n = rnd(1, it % 20 ? 9 : 16), den = rnd(0, 10);
    vector adj(n, vector<int>(n));
    FOR (i, 0, n - 1) G[i].clear();
    FOR (i, 0, n - 1) FOR (j, i + 1, n - 1) if (rnd(1, 10) <= den)
      adj[i][j] = adj[j][i] = 1, G[i].pb(j), G[j].pb(i);
    FOR (i, 0, n - 1) shuffle(G[i].begin(), G[i].end(), rng);
    build(n);
    set<array<int, 3>> want, got;
    FOR (a, 0, n - 1) FOR (b, a + 1, n - 1) FOR (c, b + 1, n - 1)
      if (adj[a][b] && adj[b][c] && adj[a][c]) want.insert({a, b, c});
    C3(n, [&](int a, int b, int c) {
      array<int, 3> t{a, b, c};
      sort(t.begin(), t.end());
      assert(got.insert(t).second); // never reported twice
    });
    assert(got == want);
    ll w4 = 0; // a < c are one diagonal, b < d the other, a the smallest
    FOR (a, 0, n - 1) FOR (c, a + 1, n - 1) {
      int common = 0;
      FOR (b, 0, n - 1) common += b != a && b != c && adj[a][b] && adj[b][c];
      w4 += common * (common - 1) / 2;
    }
    assert(w4 % 2 == 0), w4 /= 2; // each cycle has two diagonals
    assert(C4(n) == w4);
    FOR (i, 0, n - 1) assert(vis[i] == 0); // both leave vis clean
    tri += want.size(), quad += w4, cases++;
  }
  { // star with 2e5 leaves plus a 300-clique on the first vertices
    int n = 200000;
    FOR (i, 0, n - 1) G[i].clear();
    FOR (i, 1, n - 1) G[0].pb(i), G[i].pb(0);
    FOR (i, 1, 300) FOR (j, i + 1, 300) G[i].pb(j), G[j].pb(i);
    build(n);
    ll c3 = 0;
    auto st = chrono::steady_clock::now();
    C3(n, [&](int, int, int) { c3++; });
    ll c4 = C4(n);
    double sec = chrono::duration<double>(chrono::steady_clock::now() - st).count();
    assert(c3 == 301LL * 300 * 299 / 6);           // K_301
    assert(c4 == 3 * (301LL * 300 * 299 * 298 / 24)); // 3 four-cycles per K_4
    assert(sec < 20);
  }
  printf("C3C4 ok: %d random graphs (%lld triangles, %lld 4-cycles) + star/clique n=2e5\n", cases, tri, quad);
}
