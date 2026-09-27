#include "prelude.h"
#include "stress.h"
#include "6_Math/floor_ceil.cpp"

ll rf(ll a, ll b) { ll q = a / b; if (q * b != a && ((a < 0) != (b < 0))) q--; return q; }
ll rc(ll a, ll b) { return -rf(-a, b); }
void chk(int a, int b) {
  if (b == 0 || (a == INT_MIN && b == -1)) return; // UB for a / b itself
  assert(floor(a, b) == rf(a, b));
  assert(ceil(a, b) == rc(a, b));
}
int main() {
  int cases = 0;
  FOR (a, -60, 60) FOR (b, -60, 60) chk(a, b), cases++;
  vector<int> ed = {INT_MIN, INT_MIN + 1, INT_MIN + 2, -2, -1, 0, 1, 2, INT_MAX - 1, INT_MAX, 3, -3, 1 << 30, -(1 << 30)};
  for (int a : ed) for (int b : ed) chk(a, b), cases++;
  FOR (it, 1, 1000000) {
    int a = rnd(INT_MIN, INT_MAX), b = rnd(0, 1) ? rnd(-100, 100) : rnd(INT_MIN, INT_MAX);
    chk(a, b), cases++;
  }
  // double overloads from <cmath> are still reachable
  assert(floor(-1.5) == -2.0 && ceil(1.2) == 2.0);
  printf("floor_ceil: %d cases OK\n", cases);
}
