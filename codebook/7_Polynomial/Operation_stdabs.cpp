typedef vector<int> Poly;
Poly Mul(Poly a, Poly b, int bound = N) {
  int m = SZ(a) + SZ(b) - 1, n = 1;
  while (n < m) n <<= 1;
  a.resize(n), b.resize(n);
  ntt(a), ntt(b);
  FOR (i, 0, n - 1) a[i] = mul(a[i], b[i]);
  ntt(a, true), a.resize(min(m, bound));
  return a;
} // 8e2e8b
Poly Inverse(Poly a) {
  // O(NlogN), a[0] != 0
  int n = SZ(a);
  Poly res(1, Pow(a[0], mod - 2));
  for (int m = 1; m < n; m <<= 1) {
    if (n < m * 2) a.resize(m * 2);
    Poly v1(a.begin(), a.begin() + m * 2), v2 = res;
    v1.resize(m * 4), v2.resize(m * 4);
    ntt(v1), ntt(v2);
    FOR (i, 0, m * 4 - 1)
      v1[i] = mul(mul(v1[i], v2[i]), v2[i]);
    ntt(v1, true);
    res.resize(m * 2);
    FOR (i, 0, m - 1)
      res[i] = add(res[i], res[i]);
    FOR (i, 0, m * 2 - 1)
      res[i] = sub(res[i], v1[i]);
  }
  res.resize(n);
  return res;
} // 4d79c8
pair<Poly, Poly> Divide(Poly a, Poly b) {
  // a = bQ + R, O(NlogN), b.back() != 0
  int n = SZ(a), m = SZ(b), k = n - m + 1;
  if (n < m) return {{0}, a};
  Poly ra = a, rb = b;
  reverse(ra.begin(), ra.end()), ra.resize(k);
  reverse(rb.begin(), rb.end()), rb.resize(k);
  Poly Q = Mul(ra, Inverse(rb), k);
  reverse(Q.begin(), Q.end());
  Poly res = Mul(b, Q), R(m - 1);
  FOR (i, 0, m - 2)
    R[i] = sub(a[i], res[i]);
  return {Q, R};
} // 7d15e3
Poly SqrtImpl(Poly a) {
  if (a.empty()) return {0};
  int z = QuadraticResidue(a[0], mod), n = SZ(a);
  if (z == -1) return {-1};
  Poly q(1, z);
  const int inv2 = (mod + 1) / 2;
  for (int m = 1; m < n; m <<= 1) {
    if (n < m * 2) a.resize(m * 2);
    q.resize(m * 2);
    Poly f2 = Mul(q, q, m * 2);
    FOR (i, 0, m * 2 - 1)
      f2[i] = sub(f2[i], a[i]);
    f2 = Mul(f2, Inverse(q), m * 2);
    FOR (i, 0, m * 2 - 1)
      q[i] = sub(q[i], mul(f2[i], inv2));
  }
  q.resize(n);
  return q;
} // 984549
Poly Sqrt(Poly a) {
  // O(NlogN), return {-1} if not exists
  int n = SZ(a), m = 0;
  while (m < n && a[m] == 0) m++;
  if (m == n) return Poly(n);
  if (m & 1) return {-1};
  Poly s = SqrtImpl(Poly(a.begin() + m, a.end()));
  if (s[0] == -1) return {-1};
  Poly res(n);
  FOR (i, 0, SZ(s) - 1)
    res[i + m / 2] = s[i];
  return res;
} // d1acd7
Poly Derivative(Poly a) {
  int n = SZ(a);
  Poly res(n - 1);
  FOR (i, 0, n - 2)
    res[i] = mul(a[i + 1], i + 1);
  return res;
} // 001be0
Poly Integral(Poly a) {
  int n = SZ(a);
  Poly res(n + 1);
  FOR (i, 0, n - 1)
    res[i + 1] = mul(a[i], Pow(i + 1, mod - 2));
  return res;
} // 6fc53d
Poly Ln(Poly a) {
  // O(NlogN), a[0] = 1
  int n = SZ(a);
  if (n == 1) return {0};
  Poly d = Derivative(a);
  a.pop_back();
  return Integral(Mul(d, Inverse(a), n - 1));
} // 377d20
Poly Exp(Poly a) {
  // O(NlogN), a[0] = 0
  int n = SZ(a);
  Poly q(1, 1);
  a[0] = add(a[0], 1);
  for (int m = 1; m < n; m <<= 1) {
    if (n < m * 2) a.resize(m * 2);
    Poly g(a.begin(), a.begin() + m * 2), h(ALL(q));
    h.resize(m * 2), h = Ln(h);
    FOR (i, 0, m * 2 - 1)
      g[i] = sub(g[i], h[i]);
    q = Mul(g, q, m * 2);
  }
  q.resize(n);
  return q;
} // 525e8f
Poly PolyPow(Poly a, ll k) {
  int n = SZ(a), m = 0;
  Poly ans(n, 0);
  while (m < n && a[m] == 0) m++;
  if (k && m && (k >= n || k * m >= n)) return ans;
  if (m == n) return ans[0] = 1, ans;
  int lead = m * k;
  Poly b(a.begin() + m, a.end());
  int base = Pow(b[0], k), inv = Pow(b[0], mod - 2);
  FOR (i, 0, n - m - 1)
    b[i] = mul(b[i], inv);
  b = Ln(b);
  FOR (i, 0, n - m - 1)
    b[i] = mul(b[i], k % mod);
  b = Exp(b);
  FOR (i, lead, n - 1)
    ans[i] = mul(b[i - lead], base);
  return ans;
} // 7d695a
vector<int> Evaluate(Poly a, vector<int> x) {
  if (x.empty()) return {};
  int n = SZ(x);
  vector<Poly> up(n * 2);
  FOR (i, 0, n - 1)
    up[i + n] = {sub(0, x[i]), 1};
  for (int i = n - 1; i > 0; i--)
    up[i] = Mul(up[i * 2], up[i * 2 + 1]);
  vector<Poly> down(n * 2);
  down[1] = Divide(a, up[1]).second;
  FOR (i, 2, n * 2 - 1)
    down[i] = Divide(down[i >> 1], up[i]).second;
  Poly y(n);
  FOR (i, 0, n - 1) y[i] = down[i + n][0];
  return y;
} // bff354
Poly Interpolate(vector<int> x, vector<int> y) {
  int n = SZ(x);
  vector<Poly> up(n * 2);
  FOR (i, 0, n - 1)
    up[i + n] = {sub(0, x[i]), 1};
  for (int i = n - 1; i > 0; i--)
    up[i] = Mul(up[i * 2], up[i * 2 + 1]);
  Poly a = Evaluate(Derivative(up[1]), x);
  FOR (i, 0, n - 1)
    a[i] = mul(y[i], Pow(a[i], mod - 2));
  vector<Poly> down(n * 2);
  FOR (i, 0, n - 1) down[i + n] = {a[i]};
  for (int i = n - 1; i > 0; i--) {
    Poly lhs = Mul(down[i * 2], up[i * 2 + 1]);
    Poly rhs = Mul(down[i * 2 + 1], up[i * 2]);
    down[i].resize(SZ(lhs));
    FOR (j, 0, SZ(lhs) - 1)
      down[i][j] = add(lhs[j], rhs[j]);
  }
  return down[1];
} // af80e7
Poly TaylorShift(Poly a, int c) {
  // return sum a_i(x + c)^i;
  // fac[i] = i!, facp[i] = inv(i!)
  int n = SZ(a);
  FOR (i, 0, n - 1) a[i] = mul(a[i], fac[i]);
  reverse(a.begin(), a.end());
  Poly b(n);
  int w = 1;
  FOR (i, 0, n - 1)
    b[i] = mul(facp[i], w), w = mul(w, c);
  a = Mul(a, b, n), reverse(a.begin(), a.end());
  FOR (i, 0, n - 1) a[i] = mul(a[i],facp[i]);
  return a;
} // 3a3763
vector<int> SamplingShift(vector<int> a, int c, int m){
  // given f(0), f(1), ..., f(n - 1)
  // return f(c), f(c + 1), ..., f(c + m - 1)
  int n = SZ(a); // 4d649d
  FOR (i, 0, n - 1) a[i] = mul(a[i],facp[i]);
  Poly b(n);
  FOR (i, 0, n - 1) {
    b[i] = facp[i];
    if (i & 1) b[i] = sub(0, b[i]);
  }
  a = Mul(a, b, n);
  FOR (i, 0, n - 1) a[i] = mul(a[i], fac[i]);
  reverse(a.begin(), a.end());
  int w = 1;
  FOR (i, 0, n - 1)
    b[i] = mul(facp[i], w), w = mul(w, sub(c, i));
  a = Mul(a, b, n);
  reverse(a.begin(), a.end());
  FOR (i, 0, n - 1) a[i] = mul(a[i],facp[i]);
  a.resize(m), b.resize(m);
  FOR (i, 0, m - 1) b[i] = facp[i];
  a = Mul(a, b, m);
  FOR (i, 0, m - 1) a[i] = mul(a[i], fac[i]);
  return a;
} // 2e52c1
Poly power_proj(vector<int> w, Poly f, int m) {
  // return sum_j w_j[x^j]f^i for i=0,1,...,m
  int n = 1;
  while (n < SZ(f)) n <<= 1;
  f.resize(n), w.resize(n), reverse(w.begin(), w.end());
  int k = 1, n2 = 2 * n, n4 = 4 * n;
  Poly _P(n2), _Q(n2);
  FOR (i, 0, n - 1)
    _P[i] = w[i], _Q[i] = sub(0, f[i]);
  while (n > 1) {
    Poly R(n2);
    FOR (i, 0, n2 - 1)
      R[i] = (i & 1 ? sub(0, _Q[i]) : _Q[i]);
    Poly PQ = Mul(_P, R), QQ = Mul(_Q, R);
    PQ.resize(n4), QQ.resize(n4);
    for (int i = 0, j = n2; i < n2; i++, j++) {
      PQ[j] = add(PQ[j], _P[i]);
      QQ[j] = add(QQ[j], add(_Q[i], R[i]));
    }
    fill(ALL(_P), 0), fill(ALL(_Q), 0);
    FOR (j, 0, 2 * k - 1)
      FOR (i, 0, n / 2 - 1) {
        _P[n * j + i] = PQ[2 * n * j + 2 * i + 1];
        _Q[n * j + i] = QQ[2 * n * j + 2 * i + 0];
      }
    n /= 2, k *= 2;
  }
  Poly p(k);
  FOR (i, 0, k - 1) p[i] = _P[2 * i];
  reverse(p.begin(), p.end()), p.resize(m + 1);
  return p;
} // 1409fd