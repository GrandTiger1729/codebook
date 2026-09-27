#include "prelude.h"
#include "stress.h"
#include "9_Else/HilbertCurve.cpp"

int main() {
  int cases = 0;
  for (int k = 0; k <= 10; k++) {
    int n = 1 << k;
    vector<pair<int,int>> at((size_t)n * n, {-1, -1});
    FOR (x, 0, n - 1) FOR (y, 0, n - 1) {
      ll d = hilbert(n, x, y);
      assert(0 <= d && d < (ll)n * n && at[d].F == -1);  // bijection onto [0, n^2)
      at[d] = {x, y};
    }
    for (ll d = 0; d + 1 < (ll)n * n; d++)                  // consecutive cells are grid-adjacent
      assert(abs(at[d].F - at[d + 1].F) + abs(at[d].S - at[d + 1].S) == 1);
    if (k) assert(at[0] == make_pair(0, 0));
    // blocks of 4 consecutive cells are aligned 2x2 squares, visited in the order of the n/2 curve
    if (k >= 1) FOR (x, 0, n - 1) FOR (y, 0, n - 1) assert(hilbert(n, x, y) / 4 == hilbert(n / 2, x / 2, y / 2));
    cases++;
  }
  // large n: no overflow, still in range, locality
  for (int it = 0; it < 200000; it++) {
    int n = 1 << 30, x = rnd(0, n - 1), y = rnd(0, n - 1);
    ll d = hilbert(n, x, y); assert(0 <= d && d < (ll)n * n);
    if (x + 1 < n) { ll e = hilbert(n, x + 1, y); assert(e != d); }
  }
  printf("Hilbert ok: n=2^0..2^10 bijection+adjacency (%d sizes), 2e5 random at n=2^30\n", cases);
}
