#include "prelude.h"
#include "stress.h"
const int N = 405;
vector<ll> f, a, b;
ll w(int j, int i) { return f[i - j] + a[j] + b[i]; }
#include "9_Else/1d1d_segments.cpp"
// dp[i] = min_{j<i} dp[j] + w(j, i) with w(j, i) = f(i - j) + a[j] + b[i]:
// f convex gives the quadrangle inequality (convex1D1D), f concave the
// reverse one (concave1D1D). Compared with the O(n^2) DP; small values so
// that ties are common, and dp[0] random since the template reads it.
int main() {
  int cases[2] = {0, 0};
  FOR (it, 1, 20000) {
    int n = rnd(1, it % 50 ? 30 : 400), V = it % 2 ? 3 : 100;
    FOR (type, 0, 1) {
      f.assign(n + 1, 0), a.assign(n + 1, 0), b.assign(n + 1, 0);
      vector<ll> inc(n);
      for (auto &x : inc) x = rnd(-V, V);
      sort(inc.begin(), inc.end());                  // convex: increments go up
      if (type == 1) reverse(inc.begin(), inc.end()); // concave: they go down
      FOR (d, 1, n) f[d] = f[d - 1] + inc[d - 1];
      FOR (i, 0, n) a[i] = rnd(-V, V), b[i] = rnd(-V, V);
      vector<ll> want(n + 1, LLONG_MAX);
      want[0] = dp[0] = rnd(-V, V);
      FOR (i, 1, n) FOR (j, 0, i - 1) want[i] = min(want[i], want[j] + w(j, i));
      if (type == 0) convex1D1D(n); else concave1D1D(n);
      FOR (i, 0, n) assert(dp[i] == want[i]);
      cases[type]++;
    }
  }
  printf("1d1d_segments: convex %d, concave %d cases vs O(n^2) DP (n <= 400)\n", cases[0], cases[1]);
}
