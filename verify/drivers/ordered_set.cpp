#include "../prelude.h"
const int N = 1 << 20;   // BIT_kth needs N to be a power of two
#include "../../codebook/3_Data_Structure/BIT_kth.cpp"

// BIT_kth only ships the descent; the driver supplies the BIT itself.
int tot;
void add(int i, int v) { for (i++; i <= N; i += i & -i) bit[i] += v; }
int pref(int i) { int s = 0; for (i++; i > 0; i -= i & -i) s += bit[i]; return s; }

int main() {
  Waimai;
  int n, q; cin >> n >> q;
  vector<int> a(n); for (int &x : a) cin >> x;
  vector<array<int, 2>> qs(q);
  vector<int> xs = a;
  for (auto &[t, x] : qs) { cin >> t >> x; if (t != 2) xs.pb(x); }
  sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
  auto idx = [&](int v) { return int(lower_bound(xs.begin(), xs.end(), v) - xs.begin()); };
  vector<char> in(xs.size(), 0);
  for (int x : a) { int i = idx(x); if (!in[i]) in[i] = 1, add(i, 1), tot++; }
  for (auto [t, x] : qs) {
    if (t == 0) { int i = idx(x); if (!in[i]) in[i] = 1, add(i, 1), tot++; }
    else if (t == 1) { int i = idx(x); if (in[i]) in[i] = 0, add(i, -1), tot--; }
    else if (t == 2) cout << (x > tot ? -1 : xs[query_kth(x) - 1]) << '\n';
    else if (t == 3) {
      int i = int(upper_bound(xs.begin(), xs.end(), x) - xs.begin());
      cout << (i ? pref(i - 1) : 0) << '\n';
    } else if (t == 4) {
      int i = int(upper_bound(xs.begin(), xs.end(), x) - xs.begin());
      int c = i ? pref(i - 1) : 0;
      cout << (c == 0 ? -1 : xs[query_kth(c) - 1]) << '\n';
    } else {
      int i = int(lower_bound(xs.begin(), xs.end(), x) - xs.begin());
      int c = i ? pref(i - 1) : 0;
      cout << (c + 1 > tot ? -1 : xs[query_kth(c + 1) - 1]) << '\n';
    }
  }
}
