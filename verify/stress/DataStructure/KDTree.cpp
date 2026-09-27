#include "prelude.h"
#include "stress.h"
const int maxn = 2000;
struct point { int x, y; };
#include "3_Data_Structure/KDTree.cpp"

int main() {
  int cases = 0, qs = 0;
  for (int it = 0; it < 3000; it++) {
    int n = rnd(1, it % 10 ? 50 : maxn);
    int C = vector<int>{2, 10, 1000, 1000000000}[rnd(0, 3)];
    vector<point> v(n);
    for (auto &p : v) p = {(int)rnd(-C, C), (int)rnd(-C, C)};
    if (rnd(0, 3) == 0) FOR (i, 1, n - 1) if (rnd(0, 2) == 0) v[i] = v[rnd(0, i - 1)]; // duplicates
    kdt::init(v);
    FOR (q, 1, 20) {
      point Q = rnd(0, 1) ? v[rnd(0, n - 1)] : point{(int)rnd(-C, C), (int)rnd(-C, C)};
      ll want = LLONG_MAX; // nearest point at nonzero distance (up to 8e18 here)
      for (auto &p : v) {
        ll d = (ll)(p.x - Q.x) * (p.x - Q.x) + (ll)(p.y - Q.y) * (p.y - Q.y);
        if (d) want = min(want, d);
      }
      ll got = kdt::nearest(Q);
      if (want == LLONG_MAX) { assert(got >= (ll)1e18); qs++; continue; } // no other point: sentinel
      if (got != want) printf("n=%d C=%d want %lld got %lld\n", n, C, want, got), fflush(stdout), assert(0);
      qs++;
    }
    cases++;
  }
  printf("%d point sets (n<=%d, coords in [-1e9,1e9]), %d queries\n", cases, maxn, qs);
}
