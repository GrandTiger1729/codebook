template<int MAXN, int P, int RT> // MAXN = 2^k
struct Poly : vector<int> { // coefficients in [0, P)
  using vector<int>::vector;
  static NTT<MAXN, P, RT> ntt;
  int n() const { return (int)size(); } // n() >= 1
  Poly(const Poly &p, int m) : vector<int>(m) {
    copy_n(p.data(), min(p.n(), m), data());
  }
  Poly& irev() { return reverse(data(), data() + n()), *this; }
  Poly& isz(int m) { return resize(m), *this; }
  Poly& iadd(const Poly &rhs) { // n() == rhs.n()
    FOR (i, 0, n() - 1)
      if (((*this)[i] += rhs[i]) >= P) (*this)[i] -= P;
    return *this;
  }
  Poly& imul(int k) {
    FOR (i, 0, n() - 1)
      (*this)[i] = (ll)(*this)[i] * k % P;
    return *this;
  }
  Poly Mul(const Poly &rhs) const {
    int m = 1;
    while (m < n() + rhs.n() - 1) m <<= 1;
    Poly X(*this, m), Y(rhs, m);
    ntt(X), ntt(Y);
    FOR (i, 0, m - 1) X[i] = (ll)X[i] * Y[i] % P;
    ntt(X, true);
    return X.isz(n() + rhs.n() - 1);
  }
  Poly Inv() const { // (*this)[0] != 0, 1e5/55ms
    if (n() == 1) return {ntt.inv((*this)[0])};
    int m = 1;
    while (m < n() * 2) m <<= 1;
    Poly Xi = Poly(*this, (n() + 1) / 2).Inv().isz(m);
    Poly Y(*this, m);
    ntt(Xi), ntt(Y);
    FOR (i, 0, m - 1) {
      ll t = (2 - (ll)Xi[i] * Y[i]) % P;
      if ((Xi[i] = t * Xi[i] % P) < 0) Xi[i] += P;
    }
    ntt(Xi, true);
    return Xi.isz(n());
  }
  Poly _sqrt() const { // (*this)[0] is a QR, 1e5/145ms
    if (n() == 1) return {QuadraticResidue((*this)[0], P)};
    Poly X = Poly(*this, (n() + 1) / 2)._sqrt().isz(n());
    return X.iadd(Mul(X.Inv()).isz(n())).imul(P / 2 + 1);
  }
  Poly Sqrt() const { // {} when no square root exists
    int z = 0;
    while (z < n() && !(*this)[z]) z++;
    if (z == n()) return Poly(n());
    if (z & 1) return {};
    Poly A(data() + z, data() + n());
    if (QuadraticResidue(A[0], P) < 0) return {};
    Poly X = A._sqrt(), R(n());
    FOR (i, 0, X.n() - 1) R[i + z / 2] = X[i];
    return R;
  }
  pair<Poly, Poly> DivMod(const Poly &rhs) const { // (rhs.)back() != 0
    if (n() < rhs.n()) return {{0}, *this};
    const int m = n() - rhs.n() + 1;
    Poly X(rhs); X.irev().isz(m);
    Poly Y(*this); Y.irev().isz(m);
    Poly Q = Y.Mul(X.Inv()).isz(m).irev();
    X = rhs.Mul(Q), Y = *this;
    FOR (i, 0, n() - 1)
      if ((Y[i] -= X[i]) < 0) Y[i] += P;
    return {Q, Y.isz(max(1, rhs.n() - 1))};
  }
  Poly Dx() const {
    Poly ret(n() - 1);
    FOR (i, 0, ret.n() - 1)
      ret[i] = (ll)(i + 1) * (*this)[i + 1] % P;
    return ret.isz(max(1, ret.n()));
  }
  Poly Sx() const {
    Poly ret(n() + 1);
    FOR (i, 0, n() - 1)
      ret[i + 1] = (ll)ntt.inv(i + 1) * (*this)[i] % P;
    return ret;
  }
  Poly _tmul(int nn, const Poly &rhs) const {
    Poly Y = Mul(rhs).isz(n() + nn - 1);
    return Poly(Y.data() + n() - 1, Y.data() + Y.n());
  }
  vector<int> _eval(const vector<int> &x, const vector<Poly> &up) const {
    const int m = (int)x.size();
    if (!m) return {};
    vector<Poly> down(m * 2);
    // down[1] = DivMod(up[1]).S;
    // FOR (i, 2, m * 2 - 1) down[i] = down[i /
    // 2].DivMod(up[i]).S;
    down[1] = Poly(up[1]).irev().isz(n()).Inv().irev()._tmul(m, *this);
    FOR (i, 2, m * 2 - 1)
      down[i] =
        up[i ^ 1]._tmul(up[i].n() - 1, down[i / 2]);
    vector<int> y(m);
    FOR (i, 0, m - 1) y[i] = down[m + i][0];
    return y;
  }
  static vector<Poly> _tree1(const vector<int> &x) {
    const int m = (int)x.size();
    vector<Poly> up(m * 2);
    FOR (i, 0, m - 1)
      up[m + i] = {(x[i] ? P - x[i] : 0), 1};
    for (int i = m - 1; i > 0; i--) up[i] = up[i * 2].Mul(up[i * 2 + 1]);
    return up;
  }
  vector<int> Eval(const vector<int> &x) const { // 1e5, 0.6s
    auto up = _tree1(x); return _eval(x, up);
  }
  static Poly Interpolate(const vector<int> &x, const vector<int> &y) { // 1e5, 0.9s
    const int m = (int)x.size();
    vector<Poly> up = _tree1(x), down(m * 2);
    vector<int> z = up[1].Dx()._eval(x, up);
    FOR (i, 0, m - 1) z[i] = (ll)y[i] * ntt.inv(z[i]) % P;
    FOR (i, 0, m - 1) down[m + i] = {z[i]};
    for (int i = m - 1; i > 0; i--) down[i] = down[i * 2].Mul(up[i * 2 + 1]).iadd(down[i * 2 + 1].Mul(up[i * 2]));
    return down[1];
  }
  Poly Ln() const { // (*this)[0] == 1, 1e5/100ms
    return Dx().Mul(Inv()).Sx().isz(n());
  }
  Poly Exp() const { // (*this)[0] == 0, 1e5/215ms
    if (n() == 1) return {1};
    Poly X = Poly(*this, (n() + 1) / 2).Exp().isz(n());
    Poly Y = X.Ln(); Y[0] = P - 1;
    FOR (i, 0, n() - 1)
      if ((Y[i] = (*this)[i] - Y[i]) < 0) Y[i] += P;
    return X.Mul(Y).isz(n());
  }
  // M := P(P - 1). If k >= M, k := k % M + M.
  Poly Pow(ll k) const {
    int nz = 0;
    while (nz < n() && !(*this)[nz]) nz++;
    if (nz * min(k, (ll)n()) >= n()) return Poly(n());
    if (!k) return Poly(Poly {1}, n());
    Poly X(data() + nz, data() + nz + n() - nz * k);
    const int c = ntt.pw(X[0], k % (P - 1));
    return X.Ln().imul(k % P).Exp().imul(c).irev().isz(n()).irev();
  }
  static pair<vector<int>, vector<int>> _fac(int m) {
    vector<int> f(m), g(m); // f[i] = i!, g[i] = 1 / i!
    f[0] = 1;
    FOR (i, 1, m - 1) f[i] = (ll)f[i - 1] * i % P;
    g[m - 1] = ntt.inv(f[m - 1]);
    for (int i = m - 1; i > 0; i--)
      g[i - 1] = (ll)g[i] * i % P;
    return {f, g};
  }
  Poly Shift(ll c) const { // f(x) -> f(x + c)
    auto [f, g] = _fac(n());
    Poly X(*this), Y(n());
    int w = 1, d = (c % P + P) % P;
    FOR (i, 0, n() - 1) {
      X[i] = (ll)X[i] * f[i] % P;
      Y[i] = (ll)g[i] * w % P, w = (ll)w * d % P;
    }
    X = X.irev().Mul(Y).isz(n()).irev();
    FOR (i, 0, n() - 1) X[i] = (ll)X[i] * g[i] % P;
    return X;
  }
  // f(0), .., f(k-1) -> f(c), .., f(c+m-1), deg f < k
  static vector<int> SampleShift(const vector<int> &a, ll c, int m) {
    const int k = (int)a.size();
    auto [f, g] = _fac(max(k, m));
    Poly X(k), Y(k);
    FOR (i, 0, k - 1) {
      X[i] = (ll)a[i] * g[i] % P;
      Y[i] = i & 1 ? P - g[i] : g[i];
    }
    X = X.Mul(Y).isz(k);
    FOR (i, 0, k - 1) X[i] = (ll)X[i] * f[i] % P;
    X.irev();
    int w = 1;
    FOR (i, 0, k - 1) {
      Y[i] = (ll)g[i] * w % P;
      w = (ll)w * ((c - i) % P + P) % P;
    }
    X = X.Mul(Y).isz(k).irev();
    FOR (i, 0, k - 1) X[i] = (ll)X[i] * g[i] % P;
    X.isz(m), Y.isz(m);
    FOR (i, 0, m - 1) Y[i] = g[i];
    X = X.Mul(Y).isz(m);
    FOR (i, 0, m - 1) X[i] = (ll)X[i] * f[i] % P;
    return X;
  }
  // ret[i] = \sum_j w_j [x^j] f^i, f[0] == 0, 2^17/3.2s
  Poly PowerProj(vector<int> w, int m) const {
    int k = 1, nn = 1;
    while (nn < n()) nn <<= 1;
    const int n2 = nn * 2;
    Poly f(*this, nn), pp(n2), qq(n2);
    w.resize(nn), reverse(w.begin(), w.end());
    FOR (i, 0, nn - 1)
      pp[i] = w[i], qq[i] = f[i] ? P - f[i] : 0;
    while (nn > 1) {
      Poly r(n2);
      FOR (i, 0, n2 - 1)
        r[i] = i & 1 && qq[i] ? P - qq[i] : qq[i];
      Poly a = pp.Mul(r).isz(n2 * 2);
      Poly b = qq.Mul(r).isz(n2 * 2);
      FOR (i, 0, n2 - 1) {
        a[n2 + i] = ((ll)a[n2 + i] + pp[i]) % P;
        b[n2 + i] = ((ll)b[n2 + i] + qq[i] + r[i]) % P;
      }
      fill(pp.begin(), pp.end(), 0);
      fill(qq.begin(), qq.end(), 0);
      FOR (j, 0, k * 2 - 1) FOR (i, 0, nn / 2 - 1) {
        pp[nn * j + i] = a[nn * j * 2 + i * 2 + 1];
        qq[nn * j + i] = b[nn * j * 2 + i * 2];
      }
      nn /= 2, k *= 2;
    }
    Poly p(k);
    FOR (i, 0, k - 1) p[i] = pp[i * 2];
    return p.irev().isz(m + 1);
  }
  static int LinearRecursion(const vector<int> &a, const vector<int> &coef, ll n) { // a_n = \sum c_j a_(n-j)
    const int k = (int)a.size();
    assert((int)coef.size() == k + 1);
    Poly C(k + 1), W(Poly {1}, k), M = {0, 1};
    FOR (i, 1, k) C[k - i] = coef[i] ? P - coef[i] : 0;
    C[k] = 1;
    // C is fixed: invert reverse(C) once, not inside
    // each DivMod. max(2, k) >= every quotient length
    Poly Ci = Poly(C).irev().isz(max(2, k)).Inv();
    auto rem = [&](Poly X) -> Poly { // X mod C, X.n() <= 2k - 1
      if (X.n() < C.n()) return X;
      const int m = X.n() - C.n() + 1;
      Poly Q = Poly(X).irev().isz(m).Mul(Poly(Ci, m)).isz(m).irev();
      Poly R = C.Mul(Q);
      FOR (i, 0, X.n() - 1)
        if ((X[i] -= R[i]) < 0) X[i] += P;
      return X.isz(k);
    };
    while (n) {
      if (n % 2) W = rem(W.Mul(M));
      n /= 2, M = rem(M.Mul(M));
    }
    ll ret = 0;
    FOR (i, 0, k - 1) ret = (ret + (ll)W[i] * a[i]) % P;
    return ret;
  }
};
using Poly_t = Poly<131072 * 2, 998244353, 3>;
template<> decltype(Poly_t::ntt) Poly_t::ntt = {};
