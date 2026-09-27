#include "../prelude.h"
#include "../mod_helpers.h"
#include "../../codebook/7_Polynomial/NTT_stdabs.cpp"

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  int sz_ = 1;
  while (sz_ < n + m - 1) sz_ <<= 1;
  vector<int> a(sz_), b(sz_);
  for (int i = 0; i < n; i++) cin >> a[i];
  for (int i = 0; i < m; i++) cin >> b[i];
  ntt(a), ntt(b);
  for (int i = 0; i < sz_; i++) a[i] = mul(a[i], b[i]);
  ntt(a, true);
  for (int i = 0; i < n + m - 1; i++) cout << a[i] << " \n"[i == n + m - 2];
}
