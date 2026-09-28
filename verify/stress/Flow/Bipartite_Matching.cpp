#include "prelude.h"
#include "stress.h"
const int N = 10;
#include "4_Flow_Matching/Bipartite_Matching.cpp"
Bipartite_Matching bm;
// Random bipartite multigraphs (L, R <= 6). matching() vs bitmask DP over R,
// mp/mq must be a valid matching of that size. cover() (Konig) must be a
// vertex cover without duplicates, of size == matching, and equal to the
// minimum cover found by 2^(L+R) brute force.
int main() {
  int cases = 0;
  FOR (it, 1, 30000) {
    int L = rnd(0, 6), R = rnd(0, 6), m = L && R ? rnd(0, L * R + 3) : 0;
    bm.init(L, R);
    vector<pair<int, int>> E;
    vector<vector<int>> has(L, vector<int>(R, 0));
    FOR (i, 1, m) {
      int a = rnd(0, L - 1), b = rnd(0, R - 1);
      E.pb(a, b), bm.add_edge(a, b), has[a][b] = 1;
    }
    // brute max matching: dp over left vertices, mask of used right
    vector<int> dp(1 << R, -1); dp[0] = 0;
    FOR (a, 0, L - 1) {
      vector<int> nd = dp;
      FOR (msk, 0, (1 << R) - 1) if (dp[msk] >= 0)
        FOR (b, 0, R - 1) if (has[a][b] && !(msk >> b & 1))
          nd[msk | 1 << b] = max(nd[msk | 1 << b], dp[msk] + 1);
      dp = nd;
    }
    int want = *max_element(dp.begin(), dp.end());
    int got = bm.matching();
    assert(got == want);
    int cnt = 0;
    FOR (a, 0, L - 1) if (bm.mp[a] != -1) {
      int b = bm.mp[a];
      assert(0 <= b && b < R && has[a][b] && bm.mq[b] == a);
      cnt++;
    }
    FOR (b, 0, R - 1) assert(bm.mq[b] == L || bm.mp[bm.mq[b]] == b);
    assert(cnt == got);
    // cover
    vector<int> cv = bm.cover();
    assert((int)cv.size() == got);
    vector<int> in(L + R, 0);
    for (int x : cv) { assert(0 <= x && x < L + R && !in[x]); in[x] = 1; }
    for (auto [a, b] : E) assert(in[a] || in[L + b]);
    int best = L + R;
    FOR (msk, 0, (1 << (L + R)) - 1) {
      bool ok = true;
      for (auto [a, b] : E) if (!(msk >> a & 1) && !(msk >> (L + b) & 1)) { ok = false; break; }
      if (ok) best = min(best, __builtin_popcount(msk));
    }
    assert(best == (int)cv.size());
    cases++;
  }
  printf("Bipartite_Matching: %d graphs (L,R<=6, multi-edges) matching vs DP, cover() vs 2^(L+R) brute force\n", cases);
}
