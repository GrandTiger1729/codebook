#include "prelude.h"
#include "stress.h"
int N;
#include "9_Else/1d1d.cpp"

// dp[j] = min_{0<=i<j} dp[i] + w(i,j), dp[0] = 0, w(i,j) = f(j-i) + a[i] + b[j].
// concave1D1D: f concave (reverse quadrangle inequality, newer candidates win near columns)
// convex1D1D : f convex  (quadrangle inequality,         newer candidates win far columns)
vector<int> f, a, b, mydp;
int w(int i, int j) { return f[j - i] + a[i] + b[j]; }
int val(int i, int x) { return mydp[i] + w(i, x); }
// Called by the template only in `dp[j] = dp[i] + cost(i, j)`; mirror its dp (the template's dp is local).
int cost(int i, int j) { mydp[j] = mydp[i] + w(i, j); return w(i, j); }
// last x in [L, R] where candidate i is at least as good as j; x = L counts as true (w(j, j) undefined)
int search(int i, int j, int L, int R) {
  int lo = L, hi = R;
  while (lo < hi) { int mid = (lo + hi + 1) / 2; if (val(i, mid) <= val(j, mid)) lo = mid; else hi = mid - 1; }
  return lo;
}
vector<int> brute() {
  vector<int> dp(N + 1, INT_MAX); dp[0] = 0;
  FOR (j, 1, N) FOR (i, 0, j - 1) dp[j] = min(dp[j], dp[i] + w(i, j));
  return dp;
}
int main(int argc, char **argv) {
  int ok[2] = {0, 0}, bad[2] = {0, 0};
  bool strict = argc < 2;  // pass any arg to only count failures instead of asserting
  for (int it = 0; it < 20000; it++) {
    N = rnd(1, it % 50 ? 30 : 400);
    for (int type = 0; type < 2; type++) {
      f.assign(N + 1, 0); a.assign(N + 1, 0); b.assign(N + 1, 0);
      vector<int> inc(N); for (auto &x : inc) x = rnd(-50, 50);
      sort(inc.begin(), inc.end());
      if (type == 0) reverse(inc.begin(), inc.end());   // concave: nonincreasing increments
      FOR (d, 1, N) f[d] = f[d - 1] + inc[d - 1];
      FOR (i, 0, N) a[i] = rnd(-100, 100), b[i] = rnd(-100, 100);
      mydp.assign(N + 1, 0);
      if (type == 0) concave1D1D(); else convex1D1D();
      bool good = mydp == brute();
      if (strict) assert(good);
      (good ? ok : bad)[type]++;
    }
  }
  printf("1d1d: concave %d ok %d bad; convex %d ok %d bad (N<=400)\n", ok[0], bad[0], ok[1], bad[1]);
}
