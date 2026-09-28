#include "prelude.h"
#include "stress.h"
#include "9_Else/smawk.cpp"
namespace mp {  // the book's other min-plus template, as a third answer
const int INF = 1e9;
#include "9_Else/min_plus_convolution.cpp"
}
// Row minima of random matrices that meet the template's 2x2 condition
// (checked by brute force): select() prefers the strictly smaller value,
// and each row's chosen column must hold that row's minimum.
// Two families: (p_i - q_j)^2 (Monge-like, filtered by the condition) and
// the min-plus band a[k - j] + b[j] with a convex. The band's invalid cells
// get INF + (distance to the valid ones), which must always meet the
// condition (asserted); a constant INF does not, and a strict select then
// fails on its ties. On the min-plus band, min_plus_convolution.cpp (divide
// and conquer) must give the same row minima too.
const ll INF = LLONG_MAX / 4;
bool ok(const vector<vector<ll>> &M) {  // the template's comment, verbatim
  int n = M.size(), m = M[0].size();
  FOR (a, 0, n - 1) FOR (b, a + 1, n - 1) FOR (u, 0, m - 1) FOR (v, u + 1, m - 1) {
    if (M[b][u] < M[b][v] && !(M[a][u] < M[a][v])) return 0;
    if (M[b][u] == M[b][v] && !(M[a][u] <= M[a][v])) return 0;
  }
  return 1;
}
int main() {
  int tested = 0, skipped = 0, crossed = 0;
  FOR (it, 1, 30000) {
    int n = rnd(1, 9), m = rnd(1, 9);
    vector<vector<ll>> M(n, vector<ll>(m));
    vector<int> conv;              // min_plus_convolution's answer (min-plus only)
    if (it % 2) {
      vector<ll> p(n), q(m);
      for (auto &x : p) x = rnd(-6, 6);
      for (auto &x : q) x = rnd(-6, 6);
      sort(p.begin(), p.end()), sort(q.begin(), q.end());
      FOR (i, 0, n - 1) FOR (j, 0, m - 1) M[i][j] = (p[i] - q[j]) * (p[i] - q[j]);
      if (!ok(M)) { skipped++; continue; }
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
        M[k][j] = i < 0 ? INF + (-i) : i >= an ? INF + (i - an + 1) : a[i] + b[j];
      }
      assert(ok(M));               // this padding must satisfy the condition
      vector<int> ai(a.begin(), a.end()), bi(b.begin(), b.end());
      conv = mp::min_plus_convolution(ai, bi);
    }
    auto ans = smawk(n, m, [&](int r, int u, int v) { return M[r][v] < M[r][u]; });
    assert((int)ans.size() == n);
    FOR (i, 0, n - 1) {
      assert(0 <= ans[i] && ans[i] < m);
      assert(M[i][ans[i]] == *min_element(M[i].begin(), M[i].end()));
      if (!conv.empty()) assert(conv[i] == M[i][ans[i]]);
    }
    crossed += !conv.empty();
    tested++;
  }
  printf("smawk: %d matrices (n, m <= 13) meeting the condition, row minima OK; %d skipped; "
         "%d min-plus also match min_plus_convolution\n", tested, skipped, crossed);
}
