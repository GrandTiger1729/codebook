#include "prelude.h"
#include "stress.h"
// Problem: tree on nodes 0..n-1 with colours; query "distinct colours on path(u, v)".
// Preprocess per the template's comment: LCA, in/out with a shared counter from 0, ord[], inset.
const int MAXN = 64;
int n, blk, dft, in[MAXN], out[MAXN], ord[2 * MAXN], arr[MAXN], col[MAXN], par[MAXN], dep[MAXN];
bitset<MAXN> inset;
vector<int> G[MAXN];
int LCA(int u, int v) { // naive; supplied by the contestant
  while (dep[u] > dep[v]) u = par[u];
  while (dep[v] > dep[u]) v = par[v];
  while (u != v) u = par[u], v = par[v];
  return u;
}
int cnt[MAXN], distinct_ = 0, have[MAXN];
void onEvent();
void add(int x) { have[x]++; if (cnt[col[x]]++ == 0) distinct_++; onEvent(); } // arr[x] = x
void sub(int x) { have[x]--; if (--cnt[col[x]] == 0) distinct_--; onEvent(); }
#include "9_Else/Mos_Algorithm_On_Tree.cpp"

// No hook at "answer query": record an answer whenever the live node multiset equals an unanswered path.
struct Q { int u, v, want, got = -1; vector<char> on; };
vector<Q> qs;
void onEvent() {
  for (auto &q : qs) if (q.got < 0) {
    bool eq = true;
    FOR (i, 0, n - 1) if (have[i] != q.on[i]) { eq = false; break; }
    if (eq) q.got = distinct_;
  }
}
void dfs(int u, int p) {
  par[u] = p; ord[in[u] = dft++] = u;
  for (int v : G[u]) if (v != p) dep[v] = dep[u] + 1, dfs(v, u);
  ord[out[u] = dft++] = u;
}
int main() {
  int cases = 0, nq = 0;
  for (int it = 0; it < 3000; it++) {
    n = rnd(1, 30); int sig = rnd(1, 6);
    FOR (i, 0, n - 1) G[i].clear(), arr[i] = i, col[i] = rnd(0, sig - 1), have[i] = 0;
    FOR (i, 1, n - 1) { int p = it % 3 ? rnd(0, i - 1) : i - 1; G[p].pb(i); G[i].pb(p); } // random / path
    int root = rnd(0, n - 1);
    dft = 0; dep[root] = 0; dfs(root, root); par[root] = root;
    inset.reset(); fill(cnt, cnt + MAXN, 0); distinct_ = 0; qs.clear();
    vector<Query> query;
    int k = rnd(1, 40);
    FOR (t, 1, k) {
      int u = rnd(0, n - 1), v = rnd(0, n - 1), c = LCA(u, v);
      Q q{u, v, 0}; q.on.assign(n, 0);
      for (int x = u; x != c; x = par[x]) q.on[x] = 1;
      for (int x = v; x != c; x = par[x]) q.on[x] = 1;
      q.on[c] = 1;
      set<int> s; FOR (i, 0, n - 1) if (q.on[i]) s.insert(col[i]);
      q.want = s.size();
      qs.pb(q);
    }
    blk = rnd(1, 2 * n);
    for (auto &q : qs) query.pb(q.u, q.v);
    solve(query);
    for (auto &q : qs) assert(q.got == q.want);
    cases++; nq += qs.size();
  }
  printf("Mo on tree ok: %d trees, %d path distinct-count queries (n<=30, random root/blk)\n", cases, nq);
}
