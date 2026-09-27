#include "../prelude.h"
#include "../../codebook/9_Else/All_LCS.cpp"

// M, N <= 1000 and Q <= 5e5: walk a from 0 to M, and for each a answer
// that a's queries in decreasing b, feeding positions into a BIT.
const int MAXN = 1005;
int n, bit_[MAXN];
void upd(int i) { for (i++; i <= n; i += i & -i) bit_[i]++; }
int qry(int i) { // # of inserted positions < i
  int r = 0;
  for (; i > 0; i -= i & -i) r += bit_[i];
  return r;
}

int main() {
  Waimai;
  int q; string s, t;
  cin >> q >> s >> t;
  n = (int)t.size();
  int m = (int)s.size();
  vector<array<int, 4>> qs(q); // a, b, c, index
  vector<vector<int>> byA(m + 1);
  FOR (i, 0, q - 1) {
    cin >> qs[i][0] >> qs[i][1] >> qs[i][2];
    qs[i][3] = i;
    byA[qs[i][0]].pb(i);
  }
  vector<int> ans(q);
  AllLCS lcs(t);
  vector<vector<int>> at(n + 1); // h value v -> positions, v + 1 as key
  FOR (a, 0, m) {
    if (a) lcs.push(s[a - 1]);
    if (byA[a].empty()) continue;
    auto &cur = byA[a];
    sort(cur.begin(), cur.end(),
      [&](int x, int y) { return qs[x][1] > qs[y][1]; });
    FOR (v, 0, n) at[v].clear();
    fill_n(bit_, n + 1, 0);
    FOR (i, 0, n - 1) at[lcs.h[i] + 1].pb(i);
    int v = n; // everything with h >= v is already inserted
    for (int id : cur) {
      int b = qs[id][1], c = qs[id][2];
      while (v > b) {
        v--;
        for (int i : at[v + 1]) upd(i);
      }
      ans[qs[id][3]] = c > b ? c - b - qry(c) : 0;
    }
  }
  FOR (i, 0, q - 1) cout << ans[i] << '\n';
}
