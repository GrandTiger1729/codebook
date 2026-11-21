#include "../prelude.h"
#include "../mod_helpers.h"
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"

int main() {
  Waimai;
  int n; cin >> n;
  vector<int> a(n);
  for (int &x : a) cin >> x;
  auto c = BerlekampMassey(a); // 0-based
  int d = SZ(c);
  cout << d << '\n';
  FOR (i, 0, d - 1) cout << c[i] << " \n"[i == d - 1];
  if (!d) cout << '\n';
}
