// Side branch: our LinearRecursion, which hoists Inverse(rev(C)).
#include "../prelude.h"
#include "../../codebook/6_Math/QuadraticResidue.cpp"
#include "../../codebook/7_Polynomial/Number_Theory_Transform.cpp"
#include "../../codebook/7_Polynomial/Polynomial_Operation.cpp"

int main() {
  Waimai;
  int d; ll k; cin >> d >> k;
  Poly a(d), c(d + 1); // c is 1-based here
  for (int i = 0; i < d; i++) cin >> a[i];
  for (int i = 1; i <= d; i++) cin >> c[i];
  cout << LinearRecursion(a, c, k) << '\n';
}
