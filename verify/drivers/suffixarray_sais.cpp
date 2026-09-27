#include "../prelude.h"
#include "../../codebook/5_String/SAIS-C++20.cpp"

int main() {
  Waimai;
  string s; cin >> s;
  int n = s.size();
  vector<int> v(n + 1, 0);
  for (int i = 0; i < n; i++) v[i] = s[i] - 'a' + 1; // letters >= 1, sentinel 0 at the end
  auto sa = sais(v);                                     // sa[0] == n (sentinel)
  for (int i = 1; i <= n; i++) cout << sa[i] << " \n"[i == n];
}
