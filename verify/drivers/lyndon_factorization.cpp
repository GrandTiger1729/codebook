#include "../prelude.h"
#include "../../codebook/5_String/LyndonFactorization.cpp"

int main() {
  Waimai;
  string s; cin >> s;
  // LC wants the start of every factor, then |s|
  int at = 0;
  cout << 0;
  for (auto &w : duval(s)) cout << ' ' << (at += w.size());
  cout << '\n';
}
