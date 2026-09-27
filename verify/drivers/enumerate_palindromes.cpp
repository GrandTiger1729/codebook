#include "../prelude.h"
const int SIZE = 1100005;
#include "../../codebook/5_String/Manacher.cpp"

int main() {
  Waimai;
  string s; cin >> s;
  int n = s.size();
  Manacher(s);
  // padded index j+1 <-> LC index j; radius z minus the padding centre
  for (int j = 0; j <= 2 * n - 2; j++)
    cout << z[j + 1] - 1 << " \n"[j == 2 * n - 2];
}
