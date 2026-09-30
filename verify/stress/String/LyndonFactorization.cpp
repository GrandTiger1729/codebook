#include "prelude.h"
#include "stress.h"
#include "5_String/LyndonFactorization.cpp"
// The Lyndon factorization is unique, so it is enough to check the
// definition: the factors concatenate to s, each one is strictly smaller
// than all of its proper suffixes, and they are non-increasing.
void check(const string &s) {
  vector<string> w = duval(s);
  string cat;
  for (auto &x : w) {
    assert(!x.empty());
    FOR (l, 1, (int)x.size() - 1) assert(x < x.substr(l));
    cat += x;
  }
  assert(cat == s);
  FOR (i, 1, (int)w.size() - 1) assert(w[i - 1] >= w[i]);
}
int main() {
  int cases = 0;
  // exhaustive: all strings over {a,b,c} up to length 9
  FOR (n, 0, 9) {
    int total = 1;
    FOR (i, 1, n) total *= 3;
    FOR (mask, 0, total - 1) {
      string s;
      for (int i = 0, m = mask; i < n; i++, m /= 3) s += char('a' + m % 3);
      check(s), cases++;
    }
  }
  FOR (it, 1, 50000) {
    int sig = rnd(1, 4), n = rnd(1, 60), p = rnd(1, n);
    string s;
    FOR (i, 0, n - 1) // periodic with a few mutations, to get repeated factors
      s += i < p || rnd(0, 9) == 0 ? char('a' + rnd(0, sig - 1)) : s[i - p];
    check(s), cases++;
  }
  printf("LyndonFactorization ok: %d cases (exhaustive sigma=3 n<=9, random+periodic n<=60)\n", cases);
}
