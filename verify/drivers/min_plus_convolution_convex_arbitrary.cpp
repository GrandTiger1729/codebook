#include "../prelude.h"
const int INF = INT_MAX;  // a_i + b_j <= 2e9 fits in int32, INF must sit above it
#include "../../codebook/9_Else/min_plus_convolution.cpp"

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  vector<int> a(n), b(m);
  for (int &x : a) cin >> x;
  for (int &x : b) cin >> x;
  auto c = min_plus_convolution(a, b);
  for (int i = 0; i < (int)c.size(); i++) cout << c[i] << " \n"[i + 1 == (int)c.size()];
}
