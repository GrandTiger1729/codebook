#include "../prelude.h"
const int N = 1100005;
#include "../../codebook/9_Else/smawk.cpp"

// SMAWK maximises, so negate: M[k][j] = -(a[k-j] + b[j]).
int n, m;
vector<ll> A, B;
long long query(int k, int j) {
  int i = k - j;
  if (i < 0 || i >= n) return LLONG_MIN / 4;
  return -(A[i] + B[j]);
}

SMAWK sm;

int main() {
  Waimai;
  cin >> n >> m;
  A.resize(n); B.resize(m);
  for (auto &x : A) cin >> x;
  for (auto &x : B) cin >> x;
  vector<int> rows(n + m - 1), cols(m);
  iota(rows.begin(), rows.end(), 0); iota(cols.begin(), cols.end(), 0);
  sm.run(rows, cols);
  for (int k = 0; k < n + m - 1; k++)
    cout << -query(k, sm.ans[k]) << " \n"[k == n + m - 2];
}
