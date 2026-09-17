#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
// needs mod, G (a primitive root), N = 2^k, and
// add, sub, mul, Pow -- Pow's exponent must be ll,
// PolyPow passes k. Operation wants fac[], ifac[] too
struct NTT {
  int w[N];
  NTT() {
    int dw = Pow(G, (mod - 1) / N);
    w[0] = 1;
    FOR (i, 1, N - 1)
      w[i] = mul(w[i - 1], dw);
  } // 0 <= a[i] < P
  void operator()(vector<int>& a, bool inv = false) {
    int n = SZ(a);
    for (int j = 1, x = 0; j < n - 1; j++) {
      for (int k = n >> 1; (x ^= k) < k; k >>= 1);
      if (j < x) swap(a[x], a[j]);
    }
    for (int L = 2; L <= n; L <<= 1) {
      int dx = N / L, dl = L >> 1;
      for (int i = 0; i < n; i += L) {
        for (int j = i, x = 0; j < i + dl; j++, x += dx) {
          int tmp = mul(a[j + dl], w[x]);
          a[j + dl] = sub(a[j], tmp);
          a[j] = add(a[j], tmp);
        }
      }
    }
    if (inv) {
      reverse(a.begin() + 1, a.end());
      int invn = Pow(n, mod - 2);
      FOR (i, 0, n - 1)
        a[i] = mul(a[i], invn);
    }
  }
} ntt;