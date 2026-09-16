//(2^16)+1, 65537, 3
// 7*17*(2^23)+1, 998244353, 3
// 1255*(2^20)+1, 1315962881, 3
// 51*(2^25)+1, 1711276033, 29
template <int MAXN, int P, int RT> // MAXN must be 2^k
struct NTT {
  int w[MAXN];
  int mpow(int a, ll n);
  int minv(int a) { return mpow(a, P - 2); }
  NTT() {
    int dw = mpow(RT, (P - 1) / MAXN);
    w[0] = 1;
    FOR (i, 1, MAXN - 1)
      w[i] = (ll)w[i - 1] * dw % P;
  }
  void bitrev(int *a, int n) {
    int i = 0;
    FOR (j, 1, n - 2) {
      for (int k = n >> 1; (i ^= k) < k; k >>= 1);
      if (j < i) swap(a[i], a[j]);
    }
  }
  void operator()(
    int *a, int n, bool inv = false) { // 0 <= a[i] < P
    bitrev(a, n);
    for (int L = 2; L <= n; L <<= 1) {
      int dx = MAXN / L, dl = L >> 1;
      for (int i = 0; i < n; i += L) {
        for (int j = i, x = 0; j < i + dl;
          j++, x += dx) {
          int tmp = (ll)a[j + dl] * w[x] % P;
          if ((a[j + dl] = a[j] - tmp) < 0)
            a[j + dl] += P;
          if ((a[j] += tmp) >= P) a[j] -= P;
        }
      }
    }
    if (inv) {
      reverse(a + 1, a + n);
      int invn = minv(n);
      FOR (i, 0, n - 1) a[i] = (ll)a[i] * invn % P;
    }
  }
};
