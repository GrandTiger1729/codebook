#include "../prelude.h"
const ll MOD = 998244353;
ll A[200005], B[200005], C[200005]; // a_v, and the edge (b, c) from v to its parent
struct Path { ll a, b, s, cnt; };   // x -> a x + b along the path; s = sum of P above the top
struct Point { ll s, cnt; };
Path vertex(int v) { return {B[v], C[v], (B[v] * A[v] + C[v]) % MOD, 1}; }
Path add_vertex(Point p, int v) {
  return {B[v], C[v], (B[v] * (A[v] + p.s) + C[v] * (1 + p.cnt)) % MOD, 1 + p.cnt};
}
Point add_edge(Path x) { return {x.s, x.cnt}; }
Point rake(Point x, Point y) { return {(x.s + y.s) % MOD, x.cnt + y.cnt}; }
Path compress(Path t, Path b) {
  return {t.a * b.a % MOD, (t.a * b.b + t.b) % MOD,
          (t.s + t.a * b.s + t.b * (b.cnt % MOD)) % MOD, t.cnt + b.cnt};
}
#include "../../codebook/3_Data_Structure/StaticTopTree.cpp"

int main() {
  Waimai;
  int n, q; cin >> n >> q;
  FOR (i, 0, n - 1) cin >> A[i];
  vector<array<ll, 4>> e(n - 1);
  vector<vector<pair<int, int>>> adj(n);
  FOR (i, 0, n - 2) {
    auto &[u, v, b, c] = e[i];
    cin >> u >> v >> b >> c;
    adj[u].pb(v, i), adj[v].pb(u, i);
  }
  vector<vector<int>> g(n);
  vector<int> child(n - 1), ord{0}, vis(n, 0);
  B[0] = 1, C[0] = 0, vis[0] = 1;
  FOR (i, 0, n - 1) for (auto [v, id] : adj[ord[i]]) if (!vis[v]) {
    vis[v] = 1, ord.pb(v), g[ord[i]].pb(v), child[id] = v;
    B[v] = e[id][2], C[v] = e[id][3];
  }
  StaticTopTree t(g);
  while (q--) {
    int op; cin >> op;
    if (op == 0) { int w; ll x; cin >> w >> x; A[w] = x, t.update(w); }
    else { int i; ll y, z; cin >> i >> y >> z; int v = child[i]; B[v] = y, C[v] = z, t.update(v); }
    cout << t.path[t.root].s << '\n';
  }
}
