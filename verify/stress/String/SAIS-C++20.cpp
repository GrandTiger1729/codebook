#include "prelude.h"
#include "stress.h"
#include "5_String/SAIS-C++20.cpp"

// usage: letters mapped to >= 1, a unique 0 sentinel appended; result[0] == n (the sentinel)
vector<int> brute(const vector<int> &s) {
  int n = s.size(); vector<int> sa(n); iota(sa.begin(), sa.end(), 0);
  sort(sa.begin(), sa.end(), [&](int a, int b) {
    return lexicographical_compare(s.begin() + a, s.end(), s.begin() + b, s.end()); });
  return sa;
}
void check(vector<int> s) {
  vector<int> want = brute(s);
  s.pb(0);
  vector<int> got = sais(s);
  assert((int)got.size() == (int)s.size() && got[0] == (int)s.size() - 1);
  got.erase(got.begin());
  assert(got == want);
}
int main() {
  int cases = 0;
  for (int sig = 1; sig <= 3; sig++)            // exhaustive small
    for (int n = 1; n <= (sig == 1 ? 30 : sig == 2 ? 13 : 8); n++) {
      int tot = 1; FOR (i, 1, n) tot *= sig;
      for (int mask = 0; mask < tot; mask++) {
        vector<int> s; int x = mask;
        FOR (i, 1, n) s.pb(1 + x % sig), x /= sig;
        check(s); cases++;
      }
    }
  for (int it = 0; it < 4000; it++) {            // random, incl. periodic / large alphabet gaps
    int sig = it % 5 == 0 ? rnd(1, 1000) : rnd(1, 4), n = rnd(1, it % 200 == 0 ? 3000 : 200);
    vector<int> s(n);
    for (auto &c : s) c = rnd(1, sig);
    if (it % 4 == 0) { int p = rnd(1, n); FOR (i, p, n - 1) s[i] = s[i - p]; }
    check(s); cases++;
  }
  printf("SAIS ok: %d cases (exhaustive sigma<=3, random n<=3000 sigma<=1000)\n", cases);
}
