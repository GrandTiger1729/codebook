#include "prelude.h"
#include "stress.h"
#include "4_Flow_Matching/SW-mincut.cpp"

int main() {
  int cases = 0;
  FOR (it, 1, 30000) {
    int n = rnd(2, it % 3 ? 7 : 11), maxw = rnd(0, 1) ? 1 : 1000;
    vector<vector<int>> m(n, vector<int>(n, 0));
    double dens = rndd(0, 1);
    FOR (i, 0, n - 1) FOR (j, i + 1, n - 1)
      if (rndd(0, 1) < dens) m[i][j] = m[j][i] = rnd(0, maxw);
    if (it % 7 == 0) FOR (i, 0, n - 1) m[i][i] = rnd(0, 100); // self loops: must be ignored
    int best = INT_MAX;
    FOR (mask, 1, (1 << n) - 2) {
      int c = 0;
      FOR (i, 0, n - 1) FOR (j, 0, n - 1)
        if ((mask >> i & 1) && !(mask >> j & 1)) c += m[i][j];
      best = min(best, c);
    }
    auto [w, side] = globalMinCut(m);
    assert(w == best);
    // returned side must be a proper nonempty subset with that cut weight
    vector<int> in(n, 0);
    for (int v : side) assert(0 <= v && v < n && !in[v]), in[v] = 1;
    assert(!side.empty() && (int)side.size() < n);
    int c = 0;
    FOR (i, 0, n - 1) FOR (j, 0, n - 1) if (in[i] && !in[j]) c += m[i][j];
    assert(c == w);
    cases++;
  }
  printf("SW-mincut: %d cases OK\n", cases);
}
