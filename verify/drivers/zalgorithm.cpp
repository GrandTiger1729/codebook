#include "../prelude.h"
const int SIZE = 500005;
#include "../../codebook/5_String/Z-value.cpp"

int main() {
  Waimai;
  string s; cin >> s;
  int n = s.size();
  make_z(s);
  z[0] = n;
  for (int i = 0; i < n; i++) cout << z[i] << " \n"[i + 1 == n];
}
