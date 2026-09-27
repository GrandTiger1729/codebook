#include "prelude.h"
#include "stress.h"
const int N = 130;
#include "2_Graph/NumberofMaximalClique.cpp"

BronKerbosch bk;
int adj[N], n;
int main() {
  int cases = 0;
  for (int it = 0; it < 4000; it++) {
    n = rnd(1, 15);
    int dens = rnd(0, 100);
    bk.init(n);
    FOR (i, 0, n - 1) adj[i] = 1 << i;
    FOR (i, 0, n - 1) FOR (j, i + 1, n - 1) if (rnd(0, 99) < dens)
      bk.add_edge(i + 1, j + 1), adj[i] |= 1 << j, adj[j] |= 1 << i;
    // brute: S clique & no outside vertex adjacent to all of S
    vector<char> cl(1 << n);
    cl[0] = 1;
    int want = 0;
    FOR (S, 1, (1 << n) - 1) {
      int v = __builtin_ctz(S);
      cl[S] = cl[S ^ 1 << v] && (adj[v] & S) == S;
    }
    FOR (S, 1, (1 << n) - 1) if (cl[S]) {
      int com = (1 << n) - 1;
      FOR (v, 0, n - 1) if (S >> v & 1) com &= adj[v];
      want += com == S;
    }
    assert(bk.solve() == want);
    cases++;
  }
  // pruning: Moon-Moser graph K_{3,3,...} has 3^(n/3) maximal cliques; answer capped at 1001
  int capped = 0;
  for (int k = 1; k <= 40; k++) {
    n = 3 * k;
    bk.init(n);
    FOR (i, 1, n) FOR (j, i + 1, n) if ((i - 1) / 3 != (j - 1) / 3) bk.add_edge(i, j);
    ll want = 1; FOR (i, 1, k) want = min(want * 3, 100000LL);
    int got = bk.solve();
    assert(got == min(want, 1001LL));
    capped += got > 1000;
  }
  printf("%d graphs n<=15 exact; Moon-Moser n<=120 (%d capped at 1001)\n", cases, capped);
}
