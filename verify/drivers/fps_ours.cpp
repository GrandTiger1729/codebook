// Side branch: our own free-function Poly, kept tested so it stays usable.
#include "../prelude.h"
#include "../../codebook/6_Math/QuadraticResidue.cpp"
#include "../../codebook/7_Polynomial/Number_Theory_Transform.cpp"
#include "../../codebook/7_Polynomial/Polynomial_Operation.cpp"

int main() {
  Waimai;
  int n; cin >> n;
#ifdef OP_POW
  ll k; cin >> k;
#endif
  Poly a(n);
  for (int i = 0; i < n; i++) cin >> a[i];
#if defined(OP_INV)
  Poly b = Inverse(a);
#elif defined(OP_EXP)
  Poly b = Exp(a);
#elif defined(OP_POW)
  Poly b = PolyPow(a, k);
#elif defined(OP_SQRT)
  Poly b = Sqrt(a);
  if (b.empty()) { cout << "-1\n"; return 0; }
#endif
  for (int i = 0; i < n; i++) cout << b[i] << " \n"[i + 1 == n];
}
