#include "../prelude.h"
#include "../../codebook/7_Polynomial/Fast_Fourier_Transform.cpp"
#include "../../codebook/7_Polynomial/Fast_Fourier_Transform_Mod.cpp"

int main() {
  Waimai;
  int n, m; cin >> n >> m;
  vector<ll> a(n), b(m);
  for (auto &x : a) cin >> x;
  for (auto &x : b) cin >> x;
  auto c = conv_mod<1000000007>(a, b);
  for (int i = 0; i < (int)c.size(); i++)
    cout << c[i] << " \n"[i + 1 == (int)c.size()];
}
