#include "../prelude.h"
#include "../../codebook/6_Math/QuadraticResidue.cpp"

int main() {
  Waimai;
  int t; cin >> t;
  while (t--) {
    int y, p; cin >> y >> p;
    cout << QuadraticResidue(y, p) << '\n';
  }
}
