#include "../prelude.h"
#include "../../codebook/6_Math/QuadraticResidue.cpp"
#include "../mod_helpers.h"
#include "../../codebook/7_Polynomial/NTT_stdabs.cpp"
#include "../../codebook/7_Polynomial/Operation_stdabs.cpp"
#include "../../codebook/7_Polynomial/FastLinearRecursion.cpp"

int main() {
  Waimai;
  int d; ll k; cin >> d >> k;
  vector<int> a(d), c(d);
  for (int i = 0; i < d; i++) cin >> a[i];
  for (int i = 0; i < d; i++) cin >> c[i];
  cout << BostanMori(a, c, k) << '\n';
}
