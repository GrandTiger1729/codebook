// StaticTopTree query_subtree(v) and query_reroot(v) vs brute force, with the same
// non-commutative DP as StaticTopTree.cpp (each vertex is an affine map, a cluster's
// s = sum over its vertices w of (maps from the top down to w)(A[w])), so a wrong
// order or a missing / doubled piece changes the answer. Point updates in between.
// Shapes: random, path, star, caterpillar, binary-ish; n = 1 and 2 included.
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
int n;
vector<vector<int>> adj;
// sum over w reachable from v without passing `block` of (f_v o ... o f_w)(A[w]),
// plus the count; f_x(y) = B[x] y + C[x]
pair<ll, ll> brute(int v, int block) {
  ll s = 0, cnt = 0;
  // value(w) = f along the path; do it by DFS carrying the composite map top->here
  function<void(int, int, ll, ll)> dfs = [&](int x, int from, ll a, ll b) {
    ll na = a * B[x] % MOD, nb = (a * C[x] + b) % MOD; // composite o f_x
    s = (s + na * A[x] + nb) % MOD, cnt++;
    for (int y : adj[x]) if (y != from && y != block) dfs(y, x, na, nb);
  };
  dfs(v, -1, 1, 0);
  return {s, cnt};
}
int main() {
  int trees = 0, qs = 0, qr = 0;
  FOR (it, 1, 4000) {
    n = it % 50 ? rnd(1, 25) : rnd(26, 400);
    int shape = it % 5;
    vector<int> par(n, -1);
    FOR (v, 1, n - 1)
      par[v] = shape == 0 ? rnd(0, v - 1) : shape == 1 ? v - 1 : shape == 2 ? 0
             : shape == 3 ? (v % 2 ? v - 1 : max(0, v - 2)) : (v - 1) / 2;
    vector<int> perm(n); iota(perm.begin(), perm.end(), 0); shuffle(perm.begin() + 1, perm.end(), rng);
    vector<vector<int>> g(n); adj.assign(n, {});
    vector<int> P(n, -1);
    FOR (v, 1, n - 1) {
      int a = perm[par[v]], b = perm[v];
      g[a].pb(b), P[b] = a, adj[a].pb(b), adj[b].pb(a);
    }
    FOR (v, 0, n - 1) A[v] = rnd(0, MOD - 1), B[v] = rnd(0, MOD - 1), C[v] = rnd(0, MOD - 1);
    StaticTopTree t(g);
    int Q = n > 100 ? 30 : 60;
    FOR (q, 0, Q) {
      if (q % 3 == 1) {
        int v = rnd(0, n - 1);
        A[v] = rnd(0, MOD - 1);
        if (rnd(0, 1)) B[v] = rnd(0, MOD - 1), C[v] = rnd(0, MOD - 1);
        t.update(v);
      }
      int v = rnd(0, n - 1);
      auto [ss, sc] = brute(v, P[v]); // subtree of v in the original rooting
      Path r = t.query_subtree(v);
      assert(r.s == ss && r.cnt == sc), qs++;
      auto [rs, rc] = brute(v, -1); // whole tree rooted at v
      Path w = t.query_reroot(v);
      assert(w.s == rs && w.cnt == rc), qr++;
    }
    // every vertex once, so each node type on the climb is hit
    FOR (v, 0, n - 1) {
      assert(t.query_subtree(v).s == brute(v, P[v]).first);
      assert(t.query_reroot(v).s == brute(v, -1).first);
      qs++, qr++;
    }
    trees++;
  }
  printf("StaticTopTree queries ok: %d trees, %d subtree + %d reroot (n<=400)\n", trees, qs, qr);
}
