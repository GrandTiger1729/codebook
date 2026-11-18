#include "prelude.h"
#include "stress.h"
#include "5_String/SAIS-C++20.cpp"
#include "5_String/LCP_Array.cpp"

// SA from sais() (padding dropped), LCP vs direct comparison of adjacent suffixes
int main() {
  int cases = 0;
  for (int it = 0; it < 30000; it++) {
    int sig = it % 7 == 0 ? rnd(1, 300) : rnd(1, 3), n = rnd(1, it % 300 == 0 ? 2000 : 40);
    vector<int> s(n);
    for (auto &c : s) c = rnd(1, sig);
    if (it % 3 == 0) { int p = rnd(1, n); FOR (i, p, n - 1) s[i] = s[i - p]; }
    vector<int> t = s; t.pb(0);
    vector<int> sa = sais(t); sa.erase(sa.begin());
    vector<int> lcp = lcp_array(s, sa);
    assert((int)lcp.size() == n - 1);
    FOR (i, 0, n - 2) {
      int a = sa[i], b = sa[i + 1], l = 0;
      while (a + l < n && b + l < n && s[a + l] == s[b + l]) l++;
      assert(lcp[i] == l);
    }
    string str(n, 'a'); FOR (i, 0, n - 1) str[i] = 'a' + s[i] % 26; // string input compiles too
    (void)lcp_array(str, sa);
    cases++;
  }
  printf("LCP ok: %d cases (n<=2000, periodic and random)\n", cases);
}
