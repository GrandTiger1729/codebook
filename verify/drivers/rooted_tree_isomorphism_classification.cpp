#include "../prelude.h"
const int N = 500005;
vector<int> G[N];
#include "../../codebook/9_Else/tree_hash.cpp"

int main() {
  Waimai;
  int n; cin >> n;
  FOR (i, 1, n - 1) {
    int p; cin >> p;
    G[p].pb(i);
  }
  seed = chrono::steady_clock::now().time_since_epoch().count() | 1;
  dfs(0, -1);
  unordered_map<ull, int> id;
  id.reserve(n * 2);
  vector<int> a(n);
  FOR (i, 0, n - 1) {
    auto it = id.emplace(h[i], (int)id.size());
    a[i] = it.first->second;
  }
  cout << id.size() << '\n';
  FOR (i, 0, n - 1) cout << a[i] << " \n"[i + 1 == n];
  if (!n) cout << '\n';
}
