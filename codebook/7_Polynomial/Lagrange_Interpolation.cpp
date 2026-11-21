#define SZ(a) ((int)a.size())
// y[i] = f(i), i = 0..d, deg f <= d; returns f(x), x >= 0
// O(d), needs ifac[] up to d
int Lagrange(const vector<int> &y, ll x) {
  int d = SZ(y) - 1, r = 0;
  x %= MOD;
  vector<int> pre(d + 2, 1), suf(d + 2, 1);
  FOR (i, 0, d) pre[i + 1] = mul(pre[i], sub(x, i));
  for (int i = d; i >= 0; i--)
    suf[i] = mul(suf[i + 1], sub(x, i));
  FOR (i, 0, d) {
    int t = mul(mul(y[i], mul(pre[i], suf[i + 1])),
                mul(ifac[i], ifac[d - i]));
    r = (d - i) & 1 ? sub(r, t) : add(r, t);
  }
  return r;
}
