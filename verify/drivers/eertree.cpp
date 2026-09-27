#include "../prelude.h"
#include "../../codebook/5_String/PalTree.cpp"

palindromic_tree pt;

int main() {
  Waimai;
  string s; cin >> s;
  int m = s.size();
  vector<int> last(m);
  for (int i = 0; i < m; i++) { pt.add(s[i]); last[i] = pt.last; }
  int tot = (int)pt.St.size();
  // template: St[0] = EVEN, St[1] = ODD, St[2..] = palindromes in creation order.
  // LC: EVEN = 0, ODD = -1, palindromes = 1..n.  So map v -> v - 1.
  vector<int> par(tot, 0);
  for (int u = 0; u < tot; u++)
    for (int c = 0; c < 26; c++)
      if (pt.St[u].next[c]) par[pt.St[u].next[c]] = u;
  auto mp = [](int v) { return v == 0 ? 0 : v == 1 ? -1 : v - 1; };
  cout << tot - 2 << '\n';
  for (int v = 2; v < tot; v++)
    cout << mp(par[v]) << ' ' << mp(pt.St[v].fail) << '\n';
  for (int i = 0; i < m; i++) cout << mp(last[i]) << " \n"[i + 1 == m];
}
