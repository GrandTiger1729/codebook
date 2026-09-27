#include "../prelude.h"
const int N = 500005, CNUM = 26;
#include "../../codebook/5_String/exSAM.cpp"

exSAM sam;

int main() {
  Waimai;
  string s; cin >> s;
  sam.init();
  sam.insert(s);
  sam.build();
  ll ans = 0;
  for (int v = 1; v < sam.tot; v++) ans += sam.len[v] - sam.len[sam.link[v]];
  cout << ans << '\n';
}
