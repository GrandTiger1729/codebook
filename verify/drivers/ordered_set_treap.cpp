#include "../prelude.h"
#include "../../codebook/3_Data_Structure/Treap.cpp"

int main() {
  Waimai;
  int n, q; cin >> n >> q;
  node *root = nullptr;
  int tot = 0;
  set<int> have;
  for (int i = 0; i < n; i++) { int x; cin >> x; insert(root, x); have.insert(x); tot++; }
  while (q--) {
    int t, x; cin >> t >> x;
    if (t == 0) { if (!have.count(x)) insert(root, x), have.insert(x), tot++; }
    else if (t == 1) { if (have.count(x)) erase(root, x), have.erase(x), tot--; }
    else if (t == 2) cout << (x > tot ? -1 : kth(root, x)->data) << '\n';
    else if (t == 3) cout << Rank(root, x + 1) << '\n';
    else if (t == 4) { int c = Rank(root, x + 1); cout << (c == 0 ? -1 : kth(root, c)->data) << '\n'; }
    else { int c = Rank(root, x); cout << (c + 1 > tot ? -1 : kth(root, c + 1)->data) << '\n'; }
  }
}
