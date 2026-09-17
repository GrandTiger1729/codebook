//(2^16)+1, 65537, 3
// 7*17*(2^23)+1, 998244353, 3
// 1255*(2^20)+1, 1315962881, 3
// 51*(2^25)+1, 1711276033, 29
template <int MAXN, int P, int RT> // MAXN = 2^k
struct NTT {
  int w[MAXN];
  static int add(int a, int b) {
    return (a += b) >= P ? a - P : a;
  }
  static int sub(int a, int b) {
    return (a -= b) < 0 ? a + P : a;
  }
  static int mul(int a, int b) { return (ll)a * b % P; }
  static int pw(int a, ll b) {
    int r = 1;
    for (; b; b >>= 1, a = mul(a, a))
      if (b & 1) r = mul(r, a);
    return r;
  }
  static int inv(int a) { return pw(a, P - 2); }
  NTT() {
    int dw = pw(RT, (P - 1) / MAXN);
    w[0] = 1;
    FOR (i, 1, MAXN - 1) w[i] = mul(w[i - 1], dw);
  }
  void operator()(vector<int> &v, bool rev = false) {
    int n = (int)v.size(), *a = v.data();
    // n = 2^k, a[i] in [0, P)
    for (int j = 1, x = 0; j < n - 1; j++) {
      for (int k = n >> 1; (x ^= k) < k; k >>= 1);
      if (j < x) swap(a[x], a[j]);
    }
    for (int L = 2; L <= n; L <<= 1) {
      int dx = MAXN / L, dl = L >> 1;
      for (int i = 0; i < n; i += L)
        for (int j = i, x = 0; j < i + dl; j++, x += dx) {
          int u = a[j], t = mul(a[j + dl], w[x]);
          a[j] = add(u, t), a[j + dl] = sub(u, t);
        }
    }
    if (rev) {
      reverse(a + 1, a + n);
      int in = inv(n);
      FOR (i, 0, n - 1) a[i] = mul(a[i], in);
    }
  }
};
