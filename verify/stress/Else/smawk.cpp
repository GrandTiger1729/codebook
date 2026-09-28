#include "prelude.h"
#include "stress.h"
#include "9_Else/smawk.cpp"
// Random matrices that satisfy the 2x2 condition in the template's comment
// (checked by brute force), with select() preferring the strictly larger
// value; each row's chosen column must hold that row's maximum.
// That comment used to state the condition backwards (as std_abs and
// PCkomachi still do): matrices meeting the old wording broke both the
// old and the new code.
// Two families: -(p_i - q_j)^2 (Monge-like, filtered by the condition) and
// the min-plus band -(a[k - j] + b[j]) with a convex. Its out-of-band
// cells get worse the farther out, which must always meet the condition
// (asserted). With one constant -INF instead, the band does not meet it,
// and a strict select fails on those ties (81 of 15000).
const ll NEG = LLONG_MIN / 4;
bool ok(const vector<vector<ll>> &M) {
  int n = M.size(), m = M[0].size();
  FOR (a, 0, n - 1) FOR (b, a + 1, n - 1) FOR (u, 0, m - 1) FOR (v, u + 1, m - 1) {
    if (M[a][u] < M[a][v] && !(M[b][u] < M[b][v])) return 0;
    if (M[a][u] == M[a][v] && !(M[b][u] <= M[b][v])) return 0;
  }
  return 1;
}
int main() {
  int tested = 0, skipped = 0;
  FOR (it, 1, 30000) {
    int n = rnd(1, 9), m = rnd(1, 9);
    vector<vector<ll>> M(n, vector<ll>(m));
    if (it % 2) {
      vector<ll> p(n), q(m);
      for (auto &x : p) x = rnd(-6, 6);
      for (auto &x : q) x = rnd(-6, 6);
      sort(p.begin(), p.end()), sort(q.begin(), q.end());
      FOR (i, 0, n - 1) FOR (j, 0, m - 1) M[i][j] = -(p[i] - q[j]) * (p[i] - q[j]);
    } else {                       // min-plus: row k, column j, cost a[k-j] + b[j]
      int an = rnd(1, 6);          // a convex (as in LC's convex_arbitrary), b any
      n = an + m - 1, M.assign(n, vector<ll>(m));
      vector<ll> a(an), b(m);
      ll slope = rnd(-9, 0);
      a[0] = rnd(-9, 9);
      FOR (i, 1, an - 1) slope += rnd(0, 4), a[i] = a[i - 1] + slope;
      for (auto &x : b) x = rnd(-9, 9);
      FOR (k, 0, n - 1) FOR (j, 0, m - 1) {
        int i = k - j;
        M[k][j] = i < 0 ? NEG - (-i)                // worse the farther out
                : i >= an ? NEG - (i - an + 1)
                : -(a[i] + b[j]);
      }
    }
    if (!(it % 2)) assert(ok(M));   // this padding must satisfy the condition
    if (it % 2 && !ok(M)) { skipped++; continue; }
    auto ans = smawk(n, m, [&](int r, int u, int v) { return M[r][u] < M[r][v]; });
    assert((int)ans.size() == n);
    FOR (i, 0, n - 1) {
      assert(0 <= ans[i] && ans[i] < m);
      assert(M[i][ans[i]] == *max_element(M[i].begin(), M[i].end()));
    }
    tested++;
  }
  printf("smawk: %d totally monotone matrices (n, m <= 13) vs row max; %d skipped\n", tested, skipped);
}
