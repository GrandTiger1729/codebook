#include "prelude.h"
#include "stress.h"
#include "5_String/MinimumRotation.cpp"

string brute(const string &s) {
  string best = s;
  for (int i = 1; i < (int)s.size(); i++) best = min(best, s.substr(i) + s.substr(0, i));
  return best;
}
int main() {
  int cases = 0;
  // exhaustive: all strings over {a,b,c} up to length 9
  for (int n = 1; n <= 9; n++) {
    int tot = 1; FOR (i, 1, n) tot *= 3;
    for (int mask = 0; mask < tot; mask++) {
      string s; int x = mask;
      FOR (i, 1, n) s += char('a' + x % 3), x /= 3;
      assert(min_rotation(s) == brute(s)); cases++;
    }
  }
  for (int it = 0; it < 50000; it++) {
    int sig = rnd(1, 4), n = rnd(1, 60);
    string s; FOR (i, 1, n) s += char('a' + rnd(0, sig - 1));
    if (it % 3 == 0) { string p = s.substr(0, rnd(1, n)); s = ""; while ((int)s.size() < n) s += p; } // periodic
    assert(min_rotation(s) == brute(s)); cases++;
  }
  printf("MinimumRotation ok: %d cases (exhaustive sigma=3 n<=9, random+periodic n<=60)\n", cases);
}
