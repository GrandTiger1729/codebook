#include "prelude.h"
#include "stress.h"
const int N = 10;
const ll INF = 1e18;
#include "4_Flow_Matching/Kuhn_Munkres.cpp"
KM km;
// n <= m <= 7 (mostly rectangular n < m), weights up to 20 or up to 1e9.
// Sparse mode: default init (missing pair = 0), add_edge with possibly
//   negative / parallel weights -> max-weight matching, vs brute force over
//   assignments using max(0, best parallel weight).
// Perfect mode: w pre-filled with -INF (the "perfect: -INF" variant), then
//   every pair gets a weight in [-W, W] -> max-weight perfect matching.
// fl/fr must be a consistent matching whose weight is the returned value.
int main() {
  int cases = 0, rect = 0, perf = 0;
  FOR (it, 1, 20000) {
    int n = rnd(1, 7), m = rnd(n, 7);
    bool perfect = rnd(0, 2) == 0;
    ll W = rnd(0, 3) ? 20 : 1000000000;
    rect += n < m, perf += perfect;
    km.init(n, m);
    vector<vector<ll>> w(n, vector<ll>(m, 0));
    if (perfect) {
      FOR (i, 0, n - 1) fill_n(km.w[i], m, -INF);
      FOR (i, 0, n - 1) FOR (j, 0, m - 1) {
        ll x = rnd(-W, W);
        km.add_edge(i, j, x), w[i][j] = x;
      }
    } else {
      int e = rnd(0, n * m + 3);
      FOR (k, 1, e) {
        int i = rnd(0, n - 1), j = rnd(0, m - 1);
        ll x = rnd(-W / 2, W);
        km.add_edge(i, j, x), w[i][j] = max(w[i][j], x);
      }
    }
    // brute: row i picks an unused column; dp over the mask of used columns
    vector<ll> dp(1 << m, LLONG_MIN); dp[0] = 0;
    FOR (i, 0, n - 1) {
      vector<ll> nd(1 << m, LLONG_MIN);
      FOR (msk, 0, (1 << m) - 1) if (dp[msk] != LLONG_MIN)
        FOR (j, 0, m - 1) if (!(msk >> j & 1))
          nd[msk | 1 << j] = max(nd[msk | 1 << j], dp[msk] + w[i][j]);
      dp = nd;
    }
    ll want = *max_element(dp.begin(), dp.end());
    ll got = km.solve();
    assert(got == want);
    ll s = 0;
    vector<int> used(m, 0);
    FOR (i, 0, n - 1) {
      int j = km.fl[i];
      if (perfect) assert(j != -1);
      if (j == -1) continue;
      assert(0 <= j && j < m && !used[j] && km.fr[j] == i);
      used[j] = 1, s += w[i][j];
    }
    assert(s == got);
    cases++;
  }
  printf("Kuhn_Munkres: %d matrices (n<=m<=7, %d rectangular n<m, %d perfect with negatives) vs brute force over assignments\n",
    cases, rect, perf);
}
