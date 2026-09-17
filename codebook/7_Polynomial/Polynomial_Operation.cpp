#define SZ(a) ((int)a.size())
typedef vector<int> Poly; // needs NTT, coefs in [0, MOD)
Poly cut(const Poly &a, int m) { // first m coefs
  Poly r(a.begin(), a.begin() + min(m, SZ(a)));
  return r.resize(m), r;
}
Poly rev(Poly a) {
  reverse(a.begin(), a.end());
  return a;
}
Poly Mul(Poly a, Poly b) {
  int m = SZ(a) + SZ(b) - 1, n = 1;
  while (n < m) n <<= 1;
  a.resize(n), b.resize(n);
  ntt(a), ntt(b);
  FOR (i, 0, n - 1) a[i] = mul(a[i], b[i]);
  ntt(a, true);
  return a.resize(m), a;
}
Poly Inverse(Poly a) { // a[0] != 0, 1e5/50ms
  if (SZ(a) == 1) return {inv(a[0])};
  int n = SZ(a), m = 1;
  while (m < n * 2) m <<= 1;
  Poly x = cut(Inverse(cut(a, (n + 1) / 2)), m);
  a.resize(m);
  ntt(x), ntt(a);
  FOR (i, 0, m - 1)
    x[i] = mul(x[i], sub(2, mul(x[i], a[i])));
  ntt(x, true);
  return x.resize(n), x;
}
Poly sqrtQR(Poly a) { // a[0] is a QR, 1e5/150ms
  if (SZ(a) == 1) return {QuadraticResidue(a[0], MOD)};
  int n = SZ(a);
  Poly x = cut(sqrtQR(cut(a, (n + 1) / 2)), n);
  Poly y = cut(Mul(a, Inverse(x)), n);
  FOR (i, 0, n - 1)
    x[i] = mul(add(x[i], y[i]), (MOD + 1) / 2);
  return x;
}
Poly Sqrt(Poly a) { // {} when no square root exists
  int n = SZ(a), z = 0;
  while (z < n && !a[z]) z++;
  if (z == n) return Poly(n);
  if (z & 1 || QuadraticResidue(a[z], MOD) < 0) return {};
  Poly x = sqrtQR(Poly(a.begin() + z, a.end())), r(n);
  FOR (i, 0, SZ(x) - 1) r[i + z / 2] = x[i];
  return r;
}
pair<Poly, Poly> Divide(Poly a, Poly b) { // b.back() != 0
  int n = SZ(a), m = SZ(b), k = n - m + 1;
  if (n < m) return {{0}, a};
  Poly ra(a.rbegin(), a.rend()), rb(b.rbegin(), b.rend());
  ra.resize(k), rb.resize(k);
  Poly q = cut(Mul(ra, Inverse(rb)), k);
  reverse(q.begin(), q.end());
  Poly t = Mul(b, q), r(max(1, m - 1));
  FOR (i, 0, SZ(r) - 1) r[i] = sub(a[i], t[i]);
  return {q, r};
}
Poly Derivative(Poly a) {
  Poly r(max(1, SZ(a) - 1));
  FOR (i, 0, SZ(a) - 2) r[i] = mul(i + 1, a[i + 1]);
  return r;
}
Poly Integral(Poly a) {
  Poly r(SZ(a) + 1);
  FOR (i, 0, SZ(a) - 1) r[i + 1] = mul(inv(i + 1), a[i]);
  return r;
}
Poly Ln(Poly a) { // a[0] == 1, 1e5/95ms
  return cut(Integral(Mul(Derivative(a), Inverse(a))),
    SZ(a));
}
Poly Exp(Poly a) { // a[0] == 0, 1e5/240ms
  int n = SZ(a);
  if (n == 1) return {1};
  Poly x = cut(Exp(cut(a, (n + 1) / 2)), n), y = Ln(x);
  y[0] = MOD - 1;
  FOR (i, 0, n - 1) y[i] = sub(a[i], y[i]);
  return cut(Mul(x, y), n);
}
// M := MOD(MOD - 1). If k >= M, k := k % M + M.
Poly PolyPow(Poly a, ll k) { // 1e5/340ms
  int n = SZ(a), z = 0;
  while (z < n && !a[z]) z++;
  if (z * min(k, (ll)n) >= n) return Poly(n);
  if (!k) return cut(Poly {1}, n);
  Poly x(a.begin() + z, a.begin() + z + n - z * k);
  int c = pw(x[0], k % (MOD - 1));
  x = Ln(x);
  FOR (i, 0, SZ(x) - 1) x[i] = mul(x[i], k % MOD);
  x = Exp(x);
  FOR (i, 0, SZ(x) - 1) x[i] = mul(x[i], c);
  reverse(x.begin(), x.end()), x.resize(n);
  return reverse(x.begin(), x.end()), x;
}
Poly tmul(Poly a, int m, Poly b) { // middle product
  Poly y = cut(Mul(a, b), SZ(a) + m - 1);
  return Poly(y.begin() + SZ(a) - 1, y.end());
}
vector<Poly> subTree(const vector<int> &x) {
  int m = SZ(x);
  vector<Poly> up(m * 2);
  FOR (i, 0, m - 1) up[m + i] = {sub(0, x[i]), 1};
  for (int i = m - 1; i > 0; i--)
    up[i] = Mul(up[i * 2], up[i * 2 + 1]);
  return up;
}
vector<int> evalOn(Poly a, const vector<int> &x,
  const vector<Poly> &up) {
  int m = SZ(x);
  if (!m) return {};
  vector<Poly> dn(m * 2);
  // dn[1] = Divide(a, up[1]).S;
  // FOR (i, 2, m*2-1) dn[i] = Divide(dn[i/2], up[i]).S;
  dn[1] = tmul(rev(Inverse(cut(rev(up[1]), SZ(a)))), m, a);
  FOR (i, 2, m * 2 - 1)
    dn[i] = tmul(up[i ^ 1], SZ(up[i]) - 1, dn[i / 2]);
  vector<int> y(m);
  FOR (i, 0, m - 1) y[i] = dn[m + i][0];
  return y;
}
vector<int> Evaluate(Poly a, const vector<int> &x) {
  return evalOn(a, x, subTree(x)); // 1e5/0.8s
}
Poly Interpolate(const vector<int> &x,
  const vector<int> &y) { // 1e5/1.2s
  int m = SZ(x);
  vector<Poly> up = subTree(x), dn(m * 2);
  vector<int> z = evalOn(Derivative(up[1]), x, up);
  FOR (i, 0, m - 1) dn[m + i] = {mul(y[i], inv(z[i]))};
  for (int i = m - 1; i > 0; i--) {
    Poly l = Mul(dn[i * 2], up[i * 2 + 1]);
    Poly r = Mul(dn[i * 2 + 1], up[i * 2]);
    dn[i].resize(SZ(l));
    FOR (j, 0, SZ(l) - 1) dn[i][j] = add(l[j], r[j]);
  }
  return dn[1];
}
pair<Poly, Poly> facs(int m) { // i! and 1 / i!
  Poly f(m), g(m);
  f[0] = 1;
  FOR (i, 1, m - 1) f[i] = mul(f[i - 1], i);
  g[m - 1] = inv(f[m - 1]);
  for (int i = m - 1; i > 0; i--) g[i - 1] = mul(g[i], i);
  return {f, g};
}
Poly TaylorShift(Poly a, ll c) { // a(x) -> a(x + c)
  int n = SZ(a), w = 1, d = (c % MOD + MOD) % MOD;
  auto [f, g] = facs(n);
  Poly b(n);
  FOR (i, 0, n - 1) {
    a[i] = mul(a[i], f[i]);
    b[i] = mul(g[i], w), w = mul(w, d);
  }
  reverse(a.begin(), a.end());
  a = cut(Mul(a, b), n);
  reverse(a.begin(), a.end());
  FOR (i, 0, n - 1) a[i] = mul(a[i], g[i]);
  return a;
}
// a(0), .., a(k-1) -> a(c), .., a(c+m-1), deg a < k
vector<int> SamplingShift(Poly a, ll c, int m) {
  int k = SZ(a), w = 1;
  auto [f, g] = facs(max(k, m));
  Poly b(k);
  FOR (i, 0, k - 1) {
    a[i] = mul(a[i], g[i]);
    b[i] = i & 1 ? sub(0, g[i]) : g[i];
  }
  a = cut(Mul(a, b), k);
  FOR (i, 0, k - 1) a[i] = mul(a[i], f[i]);
  reverse(a.begin(), a.end());
  FOR (i, 0, k - 1) {
    b[i] = mul(g[i], w);
    w = mul(w, ((c - i) % MOD + MOD) % MOD);
  }
  a = cut(Mul(a, b), k);
  reverse(a.begin(), a.end());
  FOR (i, 0, k - 1) a[i] = mul(a[i], g[i]);
  a.resize(m), b.resize(m);
  FOR (i, 0, m - 1) b[i] = g[i];
  a = cut(Mul(a, b), m);
  FOR (i, 0, m - 1) a[i] = mul(a[i], f[i]);
  return a;
}
// ret[i] = \sum_j w_j [x^j] f^i, f[0] == 0, 2^17/2.0s
Poly PowerProj(vector<int> w, Poly f, int m) {
  int k = 1, nn = 1;
  while (nn < SZ(f)) nn <<= 1;
  const int n2 = nn * 2;
  f.resize(nn), w.resize(nn);
  reverse(w.begin(), w.end());
  Poly p(n2), q(n2);
  FOR (i, 0, nn - 1) p[i] = w[i], q[i] = sub(0, f[i]);
  while (nn > 1) {
    Poly r(n2);
    FOR (i, 0, n2 - 1)
      r[i] = i & 1 ? sub(0, q[i]) : q[i];
    Poly a = cut(Mul(p, r), n2 * 2);
    Poly b = cut(Mul(q, r), n2 * 2);
    FOR (i, 0, n2 - 1) {
      a[n2 + i] = add(a[n2 + i], p[i]);
      b[n2 + i] = add(b[n2 + i], add(q[i], r[i]));
    }
    fill(p.begin(), p.end(), 0);
    fill(q.begin(), q.end(), 0);
    FOR (j, 0, k * 2 - 1) FOR (i, 0, nn / 2 - 1) {
      p[nn * j + i] = a[nn * j * 2 + i * 2 + 1];
      q[nn * j + i] = b[nn * j * 2 + i * 2];
    }
    nn /= 2, k *= 2;
  }
  Poly ret(k);
  FOR (i, 0, k - 1) ret[i] = p[i * 2];
  reverse(ret.begin(), ret.end());
  return ret.resize(m + 1), ret;
}
// a_n = \sum_{j=1}^{k} c_j a_(n-j), c is 1-based
int LinearRecursion(const Poly &a, const Poly &c, ll n) {
  int k = SZ(a);
  Poly C(k + 1), W = cut(Poly {1}, k), M = {0, 1};
  FOR (i, 1, k) C[k - i] = sub(0, c[i]);
  C[k] = 1;
  // C is fixed: invert reverse(C) once, not inside
  // each Divide. max(2, k) >= every quotient length
  Poly Ci = Inverse(cut(rev(C), max(2, k)));
  auto rem = [&](Poly x) { // x mod C, SZ(x) <= 2k - 1
    if (SZ(x) < SZ(C)) return x;
    int m = SZ(x) - SZ(C) + 1;
    Poly q = rev(cut(Mul(cut(rev(x), m), cut(Ci, m)), m));
    Poly t = Mul(C, q);
    FOR (i, 0, SZ(x) - 1) x[i] = sub(x[i], t[i]);
    return x.resize(k), x;
  };
  while (n) {
    if (n % 2) W = rem(Mul(W, M));
    n /= 2, M = rem(Mul(M, M));
  }
  int ret = 0;
  FOR (i, 0, k - 1) ret = add(ret, mul(W[i], a[i]));
  return ret;
}
