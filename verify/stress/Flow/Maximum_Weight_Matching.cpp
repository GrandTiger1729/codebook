#include "prelude.h"
#include "stress.h"
#include "4_Flow_Matching/Maximum_Weight_Matching.cpp"
// Random graphs (n <= 12, weights in [1, 3] for many ties or [1, 1000]).
// solve() weight vs bitmask DP; match[] must be a symmetric matching on real
// edges whose weight and size equal solve()'s {weight, size}.
int main() {
  int cases = 0;
  FOR (it, 1, 30000) {
    int n = rnd(1, 12), W = it % 3 ? 1000 : 3;
    vector<vector<int>> w(n, vector<int>(n, 0));
    WeightGraph G(n);
    FOR (u, 0, n - 1) FOR (v, u + 1, n - 1) if (rnd(0, 2)) {
      w[u][v] = w[v][u] = rnd(1, W);
      G.add_edge(u + 1, v + 1, w[u][v]);
    }
    vector<ll> dp(1 << n, 0);
    FOR (m, 1, (1 << n) - 1) {
      int u = __builtin_ctz(m);
      dp[m] = dp[m ^ 1 << u];
      FOR (v, u + 1, n - 1) if (m >> v & 1 && w[u][v])
        dp[m] = max(dp[m], dp[m ^ 1 << u ^ 1 << v] + w[u][v]);
    }
    auto [tot, k] = G.solve();
    ll sum = 0; int cnt = 0;
    FOR (u, 1, n) if (int v = G.match[u]) {
      assert(G.match[v] == u && w[u - 1][v - 1] > 0);
      if (v > u) sum += w[u - 1][v - 1], cnt++;
    }
    assert(tot == dp[(1 << n) - 1] && sum == tot && cnt == k);
    cases++;
  }
  printf("Maximum_Weight_Matching ok: %d cases (n <= 12)\n", cases);
}
