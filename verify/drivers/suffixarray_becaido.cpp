#include "../prelude.h"
#include "../../codebook/5_String/SA_LCP_becaido.cpp"

int main() {
  Waimai;
  string s; cin >> s;
  auto sa = suffix_array(s);
  for (size_t i = 0; i < sa.size(); i++) cout << sa[i] << " \n"[i + 1 == sa.size()];
}
