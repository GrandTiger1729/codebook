#include "../prelude.h"
#include "../../codebook/6_Math/QuadraticResidue.cpp"
#include "../mod_helpers.h"
#include "../../codebook/7_Polynomial/Number_Theory_Transform.cpp"
#include "../../codebook/7_Polynomial/Polynomial_Operation.cpp"

int main() {
  Waimai;
  int n; cin >> n;
  Poly f(n);
  for (int &x : f) cin >> x;
  Poly g = CompInv(f);
  FOR (i, 0, n - 1) cout << g[i] << " \n"[i == n - 1];
}
