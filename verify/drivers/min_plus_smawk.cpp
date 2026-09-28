#include "../prelude.h"
#include "../../codebook/9_Else/smawk.cpp"

// M[k][j] = a[k-j] + b[j] (b convex); each row's best column is its min.
int n, m;
vector<ll> A, B;
const ll INF = LLONG_MAX / 4;
ll cost(int k, int j) {  // out of the band: worse the farther out
  int i = k - j;
  if (i < 0) return INF + (-i);
  if (i >= n) return INF + (i - n + 1);
  return A[i] + B[j];
}

int main() {
  Waimai;
  cin >> n >> m;
  A.resize(n); B.resize(m);
  for (auto &x : A) cin >> x;
  for (auto &x : B) cin >> x;
  auto ans = smawk(n + m - 1, m,
    [&](int r, int u, int v) { return cost(r, v) < cost(r, u); });
  for (int k = 0; k < n + m - 1; k++)
    cout << cost(k, ans[k]) << " \n"[k == n + m - 2];
}
