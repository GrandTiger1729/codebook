#include "../prelude.h"
const int len = 1000006, sigma = 26;
#include "../../codebook/5_String/Aho-Corasick_Automatan.cpp"

int par[len];

int main() {
  Waimai;
  int n; cin >> n;
  ac.init();
  vector<int> end(n);
  vector<string> s(n);
  for (int i = 0; i < n; i++) {
    cin >> s[i];
    // the template hardcodes c - 'A', so shift the lowercase input
    for (char &c : s[i]) c = char(c - 'a' + 'A');
    end[i] = ac.input(s[i]);
  }
  ac.make_fl();
  // the trie parent is not stored; recover it from nx[]
  for (int u = 1; u < ac.top; u++)
    for (int c = 0; c < sigma; c++)
      if (~ac.nx[u][c]) par[ac.nx[u][c]] = u;
  // codebook node v  <->  LC vertex v - 1 (root is node 1)
  cout << ac.top - 1 << '\n';
  for (int v = 2; v < ac.top; v++)
    cout << par[v] - 1 << ' ' << ac.fl[v] - 1 << '\n';
  for (int i = 0; i < n; i++) cout << end[i] - 1 << " \n"[i + 1 == n];
}
