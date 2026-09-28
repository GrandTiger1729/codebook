#include "prelude.h"
#include "stress.h"
#include "7_Polynomial/Fast_Fourier_Transform.cpp"

// conv() on random integer polynomials vs the naive O(nm) product.
// Sizes grow and shrink so fft()'s static root table is reused at every size.
int main() {
  double maxerr[2] = {0, 0}; // [V = 1e3, V = 3e4]
  int cases = 0;
  assert(conv({}, {1.0}).empty() && conv({1.0}, {}).empty());
  FOR (it, 1, 500) {
    int n = rnd(1, it % 50 == 0 ? 3000 : 300), m = rnd(1, it % 50 == 0 ? 3000 : 300);
    ll V = rnd(0, 2) ? 1000 : 30000; // keep sum a_i^2 log n inside the stated bound
    vector<double> a(n), b(m);
    vector<ll> ia(n), ib(m);
    FOR (i, 0, n - 1) a[i] = ia[i] = rnd(-V, V);
    FOR (i, 0, m - 1) b[i] = ib[i] = rnd(-V, V);
    vector<ll> want(n + m - 1);
    FOR (i, 0, n - 1) FOR (j, 0, m - 1) want[i + j] += ia[i] * ib[j];
    vector<double> got = conv(a, b);
    assert((int)got.size() == n + m - 1);
    FOR (i, 0, n + m - 2) {
      double e = fabs(got[i] - (double)want[i]);
      double &M = maxerr[V > 1000];
      M = max(M, e);
      assert(llround(got[i]) == want[i]);
    }
    cases++;
  }
  assert(maxerr[1] < 0.05);
  printf("Fast_Fourier_Transform: %d convolutions OK, max abs error %.3g (|a_i| <= 1e3), %.3g (|a_i| <= 3e4)\n", cases, maxerr[0], maxerr[1]);
}
