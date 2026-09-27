// Side branch: our own NTT.
#include "../prelude.h"
#include "../../codebook/7_Polynomial/Number_Theory_Transform.cpp"

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  int sz = 1;
  while (sz < n + m - 1) sz <<= 1;
  vector<int> a(sz), b(sz);
  for (int i = 0; i < n; i++) cin >> a[i];
  for (int i = 0; i < m; i++) cin >> b[i];
  ntt(a), ntt(b);
  for (int i = 0; i < sz; i++) a[i] = mul(a[i], b[i]);
  ntt(a, true);
  for (int i = 0; i < n + m - 1; i++) cout << a[i] << " \n"[i == n + m - 2];
}
