// StaticTopTree with the "sum over v of the composition of edge maps from root to v,
// applied to a_v" DP (Library Checker's fixed-root composite sum), vs recomputing it
// naively after every point update. Shapes: random, path, star, caterpillar, n = 1.
// Also checks the top tree's height is O(log n).
#include "prelude.h"
#include "stress.h"
const ll MOD = 998244353;
ll A[5005], B[5005], C[5005];
struct Path { ll a, b, s, cnt; };
struct Point { ll s, cnt; };
Path vertex(int v) { return {B[v], C[v], (B[v] * A[v] + C[v]) % MOD, 1}; }
Path add_vertex(Point p, int v) { return {B[v], C[v], (B[v] * (A[v] + p.s) + C[v] * (1 + p.cnt)) % MOD, 1 + p.cnt}; }
Point add_edge(Path x) { return {x.s, x.cnt}; }
Point rake(Point x, Point y) { return {(x.s + y.s) % MOD, x.cnt + y.cnt}; }
Path compress(Path t, Path b) {
  return {t.a * b.a % MOD, (t.a * b.b + t.b) % MOD, (t.s + t.a * b.s + t.b * b.cnt) % MOD, t.cnt + b.cnt};
}
#include "3_Data_Structure/StaticTopTree.cpp"
int main() {
  int cases = 0, ops = 0, maxh = 0;
  FOR (it, 1, 3000) {
    int n = rnd(1, it % 10 ? 30 : 5000), shape = it % 4;
    vector<int> par(n, -1);
    FOR (v, 1, n - 1) par[v] = shape == 0 ? rnd(0, v - 1) : shape == 1 ? v - 1 : shape == 2 ? 0 : (v % 2 ? v - 1 : max(0, v - 2));
    vector<int> perm(n); iota(perm.begin(), perm.end(), 0); shuffle(perm.begin() + 1, perm.end(), rng);
    vector<vector<int>> g(n);
    FOR (v, 1, n - 1) g[perm[par[v]]].pb(perm[v]);
    vector<int> P(n, -1); FOR (v, 1, n - 1) P[perm[v]] = perm[par[v]];
    FOR (v, 0, n - 1) A[v] = rnd(0, MOD - 1), B[v] = v ? rnd(0, MOD - 1) : 1, C[v] = v ? rnd(0, MOD - 1) : 0;
    StaticTopTree t(g);
    function<int(int)> height = [&](int k) { return k < 0 ? 0 : 1 + max(height(t.L[k]), height(t.R[k])); };
    int h = height(t.root); maxh = max(maxh, h);
    assert(h <= 6 * __lg(n) + 6);
    int Q = n > 100 ? 20 : 200;
    FOR (q, 0, Q) {
      if (q) { int v = rnd(0, n - 1); A[v] = rnd(0, MOD - 1); if (v && rnd(0, 1)) B[v] = rnd(0, MOD - 1), C[v] = rnd(0, MOD - 1); t.update(v); }
      ll want = 0;
      FOR (v, 0, n - 1) { ll x = A[v]; for (int u = v; u >= 0; u = P[u]) x = (B[u] * x + C[u]) % MOD; want += x; }
      assert(t.path[t.root].s == want % MOD);
      ops++;
    }
    cases++;
  }
  printf("StaticTopTree ok: %d trees, %d queries (n<=5000, max height %d)\n", cases, ops, maxh);
}
