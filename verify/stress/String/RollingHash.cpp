// Hash: get(l, r) equal iff the substrings are equal, on random strings over 1-3 letters
// (every pair of substrings of the same length), for string and vector<int> input.
#include "prelude.h"
#include "stress.h"
#include "5_String/RollingHash.cpp"
int main() {
  long long pairs = 0;
  FOR (it, 1, 400) {
    int n = rnd(1, 40), sig = rnd(1, 3);
    string s(n, 'a');
    for (char &c : s) c = 'a' + rnd(0, sig - 1);
    vector<int> v(s.begin(), s.end());
    Hash h(s), hv(v);
    FOR (len, 1, n) FOR (a, 0, n - len) FOR (b, 0, n - len) {
      bool eq = s.compare(a, len, s, b, len) == 0;
      assert((h.get(a, a + len) == h.get(b, b + len)) == eq);
      assert(hv.get(a, a + len) == h.get(a, a + len));
      pairs++;
    }
  }
  printf("RollingHash ok: %lld substring pairs\n", pairs);
}
