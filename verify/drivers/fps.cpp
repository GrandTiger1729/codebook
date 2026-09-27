// One driver for the FPS problems; OP is set at compile time.
#include "../prelude.h"
#include "../../codebook/6_Math/QuadraticResidue.cpp"
#include "../mod_helpers.h"
#include "../../codebook/7_Polynomial/NTT_stdabs.cpp"
#include "../../codebook/7_Polynomial/Operation_stdabs.cpp"

int main() {
  Waimai;
#if defined(OP_SHIFT) || defined(OP_SAMPLE)
  build();
#endif
  int n; cin >> n;
#ifdef OP_POW
  ll k; cin >> k;
#endif
#ifdef OP_SHIFT
  int c; cin >> c;
#endif
#ifdef OP_SAMPLE
  int m, c; cin >> m >> c;
#endif
  Poly a(n);
  for (int i = 0; i < n; i++) cin >> a[i];
#if defined(OP_INV)
  Poly b = Inverse(a);
#elif defined(OP_EXP)
  Poly b = Exp(a);
#elif defined(OP_LOG)
  Poly b = Ln(a);
#elif defined(OP_POW)
  Poly b = PolyPow(a, k);
#elif defined(OP_SQRT)
  Poly b = Sqrt(a);
  if (SZ(b) && b[0] == -1) { cout << "-1\n"; return 0; }
#elif defined(OP_SHIFT)
  Poly b = TaylorShift(a, c);
#elif defined(OP_SAMPLE)
  Poly b = SamplingShift(a, c, m);
  n = m;
#endif
  for (int i = 0; i < n; i++) cout << b[i] << " \n"[i + 1 == n];
}
