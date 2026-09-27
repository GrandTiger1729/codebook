#include "prelude.h"
#include "stress.h"
const int MAXN = 105; // > |B|
#include "5_String/KMP.cpp"

int main() {
  int cases = 0;
  for (int it = 0; it < 200000; it++) {
    int sig = rnd(1, 3), n = rnd(0, 30), m = rnd(1, 8);
    if (it % 10 == 0) m = rnd(1, 40);
    string A, B;
    FOR (i, 1, n) A += char('a' + rnd(0, sig - 1));
    FOR (i, 1, m) B += char('a' + rnd(0, sig - 1));
    vector<int> want;
    for (int i = 0; i + m <= n; i++) if (A.compare(i, m, B) == 0) want.pb(i);
    assert(match(A, B) == want);
    cases++;
  }
  printf("KMP ok: %d random cases (sigma<=3, |A|<=30, |B|<=40)\n", cases);
}
