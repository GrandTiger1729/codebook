#define sz(a) ((int)a.size())
#define all(a) a.begin(), a.end()
const int mod = 998244353, G = 3;
const int N = 1 << 20; // N must be 2^k
int add(int a, int b) {
  a += b;
  if (a >= mod) a -= mod;
  return a;
}
int sub(int a, int b) {
  a -= b;
  if (a < 0) a += mod;
  return a;
}
int mul(int a, int b) { return 1ll * a * b % mod; }
int Pow(int a, ll b) { // b is ll: PolyPow passes k
  int r = 1;
  for (; b; b >>= 1, a = mul(a, a))
    if (b & 1) r = mul(r, a);
  return r;
}
int fac[N], facp[N]; // build() before TaylorShift or
void build() { // SamplingShift
  fac[0] = 1;
  FOR (i, 1, N - 1) fac[i] = mul(fac[i - 1], i);
  facp[N - 1] = Pow(fac[N - 1], mod - 2);
  for (int i = N - 1; i; i--)
    facp[i - 1] = mul(facp[i], i);
}
// mul, add, sub, Pow
struct NTT {
  int w[N];
  NTT() {
    int dw = Pow(G, (mod - 1) / N);
    w[0] = 1;
    for (int i = 1; i < N; ++i)
      w[i] = mul(w[i - 1], dw);
  } // 0 <= a[i] < P
  void operator()(vector<int>& a, bool inv = false) {
    int n = sz(a);
    for (int j = 1, x = 0; j < n - 1; ++j) {
      for (int k = n >> 1; (x ^= k) < k; k >>= 1);
      if (j < x) swap(a[x], a[j]);
    }
    for (int L = 2; L <= n; L <<= 1) {
      int dx = N / L, dl = L >> 1;
      for (int i = 0; i < n; i += L) {
        for (int j = i, x = 0; j < i + dl; ++j, x += dx) {
          int tmp = mul(a[j + dl], w[x]);
          a[j + dl] = sub(a[j], tmp);
          a[j] = add(a[j], tmp);
        }
      }
    }
    if (inv) {
      reverse(1 + all(a));
      int invn = Pow(n, mod - 2);
      for (int i = 0; i < n; ++i)
        a[i] = mul(a[i], invn);
    }
  }
} ntt;