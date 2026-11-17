#include "../prelude.h"
#include "../../codebook/5_String/SAIS-C++20.cpp"
#include "../../codebook/5_String/LCP_Array.cpp"

int main() {
  Waimai;
  string s; cin >> s;
  ll n = s.size();
  vector<int> v(n + 1, 0);
  FOR (i, 0, n - 1) v[i] = s[i] - 'a' + 1;
  auto sa = sais(v);
  sa.erase(sa.begin());
  ll ans = n * (n + 1) / 2;
  for (int x : lcp_array(s, sa)) ans -= x;
  cout << ans << '\n';
}
