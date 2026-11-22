#include "prelude.h"
#include "stress.h"
const int MOD = 1000000009; // luogu P6097
int add(int a, int b) { a += b; if (a >= MOD) a -= MOD; return a; }
int mul(int a, int b) { return 1ll * a * b % MOD; }
#include "7_Polynomial/Fast_Walsh_Transform.cpp"

// subset_convolution vs the naive O(4^L) double loop, mod MOD, values up to MOD - 1,
// many calls in a row on the same globals (f/g/h/ct), with L changing up and down.
int A[1 << 6], B[1 << 6], C[1 << 6], W[1 << 6];
int main() {
  int cases = 0;
  FOR (it, 1, 3000) {
    int L = rnd(0, 6), n = 1 << L;
    int V = it % 3 == 0 ? 3 : MOD - 1;
    FOR (i, 0, n - 1) A[i] = rnd(0, V), B[i] = rnd(0, V);
    subset_convolution(A, B, C, L);
    FOR (k, 0, n - 1) {
      ll w = 0;
      FOR (i, 0, n - 1) FOR (j, 0, n - 1)
        if ((i | j) == k && !(i & j)) w = (w + (ll)A[i] * B[j]) % MOD;
      assert(C[k] == w);
    }
    // plain or-transform: fwt(1) is the subset sum, fwt(MOD - 1) undoes it
    FOR (i, 0, n - 1) W[i] = A[i];
    fwt(W, n, 1);
    FOR (k, 0, n - 1) {
      ll s = 0;
      FOR (i, 0, n - 1) if ((i & k) == i) s += A[i];
      assert(W[k] == s % MOD);
    }
    fwt(W, n, MOD - 1);
    FOR (i, 0, n - 1) assert(W[i] == A[i]);
    cases++;
  }
  // largest size the globals allow: L = N, compare a few entries against brute force
  {
    int L = N, n = 1 << L;
    static int a[1 << N], b[1 << N], c[1 << N];
    FOR (i, 0, n - 1) a[i] = rnd(0, MOD - 1), b[i] = rnd(0, MOD - 1);
    subset_convolution(a, b, c, L);
    FOR (t, 1, 20) {
      int k = rnd(0, n - 1);
      ll w = 0;
      for (int i = k; ; i = (i - 1) & k) { w = (w + (ll)a[i] * b[k ^ i]) % MOD; if (!i) break; }
      assert(c[k] == w);
    }
  }
  printf("Fast_Walsh_Transform: %d subset convolutions mod 1e9+9 (L=0..6, values < MOD) + L=%d spot check OK\n", cases, N);
}
