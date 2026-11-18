// josephus(n, k) vs simulation (n <= 300, k <= 400) and vs the O(n) recurrence (n <= 1e5).
#include "prelude.h"
#include "stress.h"
#include "9_Else/Josephus.cpp"
int main() {
  int cases = 0;
  FOR (n, 1, 300) FOR (k, 1, 400) {
    if (k > 60 && k % 37) continue;
    vector<int> a(n); iota(a.begin(), a.end(), 0);
    int pos = 0;
    while (a.size() > 1) pos = (pos + k - 1) % a.size(), a.erase(a.begin() + pos);
    assert(josephus(n, k) == a[0]); cases++;
  }
  FOR (it, 1, 300) {
    ll n = rnd(1, 100000), k = rnd(1, it % 2 ? 50 : 100000), s = 0;
    FOR (i, 2, n) s = (s + k) % i;
    assert(josephus(n, k) == s); cases++;
  }
  printf("Josephus ok: %d cases\n", cases);
}
