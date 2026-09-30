#include "../prelude.h"
const int N = 1000006, C = 26;
#include "../../codebook/5_String/Aho-Corasick_Automatan.cpp"

int par[N];

int main() {
  Waimai;
  int n; cin >> n;
  vector<int> end(n);
  for (int i = 0; i < n; i++) {
    string s; cin >> s;
    end[i] = ac.insert(s);
  }
  ac.build_fail();
  // the trie parent is not stored; recover it from ch[]
  for (int u = 0; u < ac._id; u++)
    for (int c = 0; c < C; c++)
      if (ac.ch[u][c]) par[ac.ch[u][c]] = u;
  // node ids are already LC's: root 0, numbered in insertion order
  cout << ac._id << '\n';
  for (int v = 1; v < ac._id; v++)
    cout << par[v] << ' ' << ac.fail[v] << '\n';
  for (int i = 0; i < n; i++) cout << end[i] << " \n"[i + 1 == n];
}
