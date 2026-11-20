#include "../prelude.h"
#include "../../codebook/6_Math/QuadraticResidue.cpp"
#include "../mod_helpers.h"
#include "../../codebook/7_Polynomial/NTT_stdabs.cpp"
#include "../../codebook/7_Polynomial/Operation_stdabs.cpp"
#include "../../codebook/7_Polynomial/CompositionalInverse.cpp"

int main() {
  Waimai;
  int n; cin >> n;
  Poly f(n);
  for (int &x : f) cin >> x;
  Poly g = CompInv(f);
  FOR (i, 0, n - 1) cout << g[i] << " \n"[i == n - 1];
}
