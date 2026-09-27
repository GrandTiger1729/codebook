#include "prelude.h"
#include "stress.h"
const int N = 64;
int par[N], dep[N], dfn[N]; // contestant-supplied tree info
vector<int> ch[N];
int LCA(int a, int b) {
  while (a != b) dep[a] > dep[b] ? a = par[a] : b = par[b];
  return a;
}
#include "2_Graph/Virtual_Tree.cpp"

int n, T;
void pre(int u) { dfn[u] = T++; for (int v : ch[u]) dep[v] = dep[u] + 1, pre(v); }
bool isAnc(int a, int b) { while (dep[b] > dep[a]) b = par[b]; return a == b; }
int main() {
  int cases = 0;
  for (int it = 0; it < 30000; it++) {
    n = rnd(1, 40);
    FOR (i, 0, n - 1) ch[i].clear();
    // random labels, root = 0
    vector<int> lab(n); iota(lab.begin(), lab.end(), 0); shuffle(lab.begin() + 1, lab.end(), rng);
    FOR (i, 1, n - 1) {
      int p = rnd(0, 2) ? rnd(max(0, i - 3), i - 1) : rnd(0, i - 1);
      par[lab[i]] = lab[p], ch[lab[p]].pb(lab[i]);
    }
    par[0] = -1, dep[0] = 0, T = 0, pre(0);
    bool withRoot = it & 1; // luogu P2495 style: key set always contains the root
    FOR (q, 1, 5) {
      vector<int> v(n); iota(v.begin(), v.end(), 0); shuffle(v.begin(), v.end(), rng);
      v.resize(rnd(1, n));
      if (withRoot && find(v.begin(), v.end(), 0) == v.end()) v.pb(0);
      // brute: closure under LCA, parent = nearest proper ancestor in the set
      set<int> S(v.begin(), v.end());
      for (int a : v) for (int b : v) S.insert(LCA(a, b));
      vector<pair<int, int>> want;
      for (int x : S) for (int y = par[x] >= 0 ? par[x] : -1; y >= 0; y = par[y])
        if (S.count(y)) { want.pb(y, x); break; }
      // replay solve() up to "do something" to read the virtual tree
      vector<int> w = v;
      top = -1;
      sort(w.begin(), w.end(), [&](int a, int b) { return dfn[a] < dfn[b]; });
      for (int i : w) insert(i);
      while (top > 0) vG[st[top - 1]].pb(st[top]), --top;
      vector<pair<int, int>> got;
      FOR (x, 0, n - 1) for (int y : vG[x]) got.pb(x, y);
      sort(want.begin(), want.end()), sort(got.begin(), got.end());
      assert(got == want);
      FOR (x, 0, n - 1) vG[x].clear();
      // the real solve(): afterwards vG must be clean for the next query
      vector<int> v2 = v;
      solve(v2);
      FOR (x, 0, n - 1) if (!vG[x].empty()) {
        printf("vG[%d] not cleared (root in set: %d, keys:", x, (int)S.count(0));
        for (int k : v) printf(" %d", k);
        printf(")\n"), fflush(stdout), assert(0);
      }
      cases++;
    }
  }
  printf("%d queries, n<=40, with and without root in key set\n", cases);
}
