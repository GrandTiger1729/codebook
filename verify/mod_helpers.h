const int mod = 998244353, G = 3;
const int N = 1 << 20; // must be 2^k
// The arithmetic NTT_stdabs.cpp no longer ships -- "can be memorised",
// so the drivers write it out the way a contestant would.
int add(int a, int b) { a += b; if (a >= mod) a -= mod; return a; }
int sub(int a, int b) { a -= b; if (a < 0) a += mod; return a; }
int mul(int a, int b) { return 1ll * a * b % mod; }
int Pow(int a, ll b) { // ll: PolyPow passes k directly
  int r = 1;
  for (; b; b >>= 1, a = mul(a, a)) if (b & 1) r = mul(r, a);
  return r;
}
int fac[N], ifac[N];
void build() {
  fac[0] = 1;
  for (int i = 1; i < N; i++) fac[i] = mul(fac[i - 1], i);
  ifac[N - 1] = Pow(fac[N - 1], mod - 2);
  for (int i = N - 1; i; i--) ifac[i - 1] = mul(ifac[i], i);
}
