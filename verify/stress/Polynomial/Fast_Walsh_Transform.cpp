#include "prelude.h"
#include "stress.h"
#include "7_Polynomial/Fast_Walsh_Transform.cpp"

// subset_convolution vs the naive O(4^L) double loop, many calls in a row on the
// same globals (f/g/h/ct), with L changing up and down between calls.
// Values stay small: h accumulates in int, and signed overflow is UB.
int A[1 << 5], B[1 << 5], C[1 << 5], W[1 << 5];
int main() {
  int cases = 0;
  FOR (it, 1, 2000) {
    int L = rnd(1, 5), n = 1 << L;
    int V = rnd(0, 1) ? 1 : 100;
    FOR (i, 0, n - 1) A[i] = rnd(-V, V), B[i] = rnd(-V, V);
    subset_convolution(A, B, C, L);
    FOR (k, 0, n - 1) {
      ll w = 0;
      FOR (i, 0, n - 1) FOR (j, 0, n - 1)
        if ((i | j) == k && !(i & j)) w += (ll)A[i] * B[j];
      assert(C[k] == w);
    }
    // plain or-transform: fwt(+1) is the subset-sum (zeta), fwt(-1) undoes it
    FOR (i, 0, n - 1) W[i] = A[i];
    fwt(W, n, 1);
    FOR (k, 0, n - 1) {
      ll s = 0;
      FOR (i, 0, n - 1) if ((i & k) == i) s += A[i];
      assert(W[k] == s);
    }
    fwt(W, n, -1);
    FOR (i, 0, n - 1) assert(W[i] == A[i]);
    cases++;
  }
  printf("Fast_Walsh_Transform: %d subset convolutions (L=1..5, shared globals) OK\n", cases);
}
