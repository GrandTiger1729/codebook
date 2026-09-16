template<int MAXN, ll P, ll RT> // MAXN = 2^k
struct Poly : vector<ll> { // coefficients in [0, P)
  using vector<ll>::vector;
  static NTT<MAXN, P, RT> ntt;
  int n() const { return (int)size(); } // n() >= 1
  Poly(const Poly &p, int m) : vector<ll>(m) {
    copy_n(p.data(), min(p.n(), m), data());
  }
  Poly& irev() { return reverse(data(), data() + n()), *this; }
  Poly& isz(int m) { return resize(m), *this; }
  Poly& iadd(const Poly &rhs) { // n() == rhs.n()
    FOR (i, 0, n() - 1)
      if (((*this)[i] += rhs[i]) >= P) (*this)[i] -= P;
    return *this;
  }
  Poly& imul(ll k) {
    FOR (i, 0, n() - 1)
      (*this)[i] = (*this)[i] * k % P;
    return *this;
  }
  Poly Mul(const Poly &rhs) const {
    int m = 1;
    while (m < n() + rhs.n() - 1) m <<= 1;
    Poly X(*this, m), Y(rhs, m);
    ntt(X.data(), m), ntt(Y.data(), m);
    FOR (i, 0, m - 1) X[i] = X[i] * Y[i] % P;
    ntt(X.data(), m, true);
    return X.isz(n() + rhs.n() - 1);
  }
  Poly Inv() const { // (*this)[0] != 0, 1e5/95ms
    if (n() == 1) return {ntt.minv((*this)[0])};
    int m = 1;
    while (m < n() * 2) m <<= 1;
    Poly Xi = Poly(*this, (n() + 1) / 2).Inv().isz(m);
    Poly Y(*this, m);
    ntt(Xi.data(), m), ntt(Y.data(), m);
    FOR (i, 0, m - 1) {
      Xi[i] *= (2 - Xi[i] * Y[i]) % P;
      if ((Xi[i] %= P) < 0) Xi[i] += P;
    }
    ntt(Xi.data(), m, true);
    return Xi.isz(n());
  }
  Poly _sqrt() const { // (*this)[0] is a QR, 1e5/235ms
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
      ret[i] = (i + 1) * (*this)[i + 1] % P;
    return ret.isz(max(1, ret.n()));
  }
  Poly Sx() const {
    Poly ret(n() + 1);
    FOR (i, 0, n() - 1)
      ret[i + 1] = ntt.minv(i + 1) * (*this)[i] % P;
    return ret;
  }
  Poly _tmul(int nn, const Poly &rhs) const {
    Poly Y = Mul(rhs).isz(n() + nn - 1);
    return Poly(Y.data() + n() - 1, Y.data() + Y.n());
  }
  vector<ll> _eval(const vector<ll> &x, const vector<Poly> &up) const {
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
    vector<ll> y(m);
    FOR (i, 0, m - 1) y[i] = down[m + i][0];
    return y;
  }
  static vector<Poly> _tree1(const vector<ll> &x) {
    const int m = (int)x.size();
    vector<Poly> up(m * 2);
    FOR (i, 0, m - 1)
      up[m + i] = {(x[i] ? P - x[i] : 0), 1};
    for (int i = m - 1; i > 0; i--) up[i] = up[i * 2].Mul(up[i * 2 + 1]);
    return up;
  }
  vector<ll> Eval(const vector<ll> &x) const { // 1e5, 1s
    auto up = _tree1(x); return _eval(x, up);
  }
  static Poly Interpolate(const vector<ll> &x, const vector<ll> &y) { // 1e5, 1.4s
    const int m = (int)x.size();
    vector<Poly> up = _tree1(x), down(m * 2);
    vector<ll> z = up[1].Dx()._eval(x, up);
    FOR (i, 0, m - 1) z[i] = y[i] * ntt.minv(z[i]) % P;
    FOR (i, 0, m - 1) down[m + i] = {z[i]};
    for (int i = m - 1; i > 0; i--) down[i] = down[i * 2].Mul(up[i * 2 + 1]).iadd(down[i * 2 + 1].Mul(up[i * 2]));
    return down[1];
  }
  Poly Ln() const { // (*this)[0] == 1, 1e5/170ms
    return Dx().Mul(Inv()).Sx().isz(n());
  }
  Poly Exp() const { // (*this)[0] == 0, 1e5/360ms
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
    while (nz < n() && !(*this)[nz]) ++nz;
    if (nz * min(k, (ll)n()) >= n()) return Poly(n());
    if (!k) return Poly(Poly {1}, n());
    Poly X(data() + nz, data() + nz + n() - nz * k);
    const ll c = ntt.mpow(X[0], k % (P - 1));
    return X.Ln().imul(k % P).Exp().imul(c).irev().isz(n()).irev();
  }
  static pair<vector<ll>, vector<ll>> _fac(int m) {
    vector<ll> f(m), g(m); // f[i] = i!, g[i] = 1 / i!
    f[0] = 1;
    FOR (i, 1, m - 1) f[i] = f[i - 1] * i % P;
    g[m - 1] = ntt.minv(f[m - 1]);
    for (int i = m - 1; i > 0; i--)
      g[i - 1] = g[i] * i % P;
    return {f, g};
  }
  Poly Shift(ll c) const { // f(x) -> f(x + c)
    auto [f, g] = _fac(n());
    Poly X(*this), Y(n());
    ll w = 1;
    c = (c % P + P) % P;
    FOR (i, 0, n() - 1) {
      X[i] = X[i] * f[i] % P;
      Y[i] = g[i] * w % P, w = w * c % P;
    }
    X = X.irev().Mul(Y).isz(n()).irev();
    FOR (i, 0, n() - 1) X[i] = X[i] * g[i] % P;
    return X;
  }
  // f(0), .., f(k-1) -> f(c), .., f(c+m-1), deg f < k
  static vector<ll> SampleShift(const vector<ll> &a, ll c, int m) {
    const int k = (int)a.size();
    auto [f, g] = _fac(max(k, m));
    Poly X(k), Y(k);
    FOR (i, 0, k - 1) {
      X[i] = a[i] * g[i] % P;
      Y[i] = i & 1 ? P - g[i] : g[i];
    }
    X = X.Mul(Y).isz(k);
    FOR (i, 0, k - 1) X[i] = X[i] * f[i] % P;
    X.irev();
    ll w = 1;
    FOR (i, 0, k - 1)
      Y[i] = g[i] * w % P, w = w * ((c - i) % P + P) % P;
    X = X.Mul(Y).isz(k).irev();
    FOR (i, 0, k - 1) X[i] = X[i] * g[i] % P;
    X.isz(m), Y.isz(m);
    FOR (i, 0, m - 1) Y[i] = g[i];
    X = X.Mul(Y).isz(m);
    FOR (i, 0, m - 1) X[i] = X[i] * f[i] % P;
    return X;
  }
  // ret[i] = \sum_j w_j [x^j] (*this)^i, i = 0..m
  // (*this)[0] == 0, 2^17/5.2s
  Poly PowerProj(vector<ll> w, int m) const {
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
        a[n2 + i] = (a[n2 + i] + pp[i]) % P;
        b[n2 + i] = (b[n2 + i] + qq[i] + r[i]) % P;
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
  static ll LinearRecursion(const vector<ll> &a, const vector<ll> &coef, ll n) { // a_n = \sum c_j a_(n-j)
    const int k = (int)a.size();
    assert((int)coef.size() == k + 1);
    Poly C(k + 1), W(Poly {1}, k), M = {0, 1};
    FOR (i, 1, k) C[k - i] = coef[i] ? P - coef[i] : 0;
    C[k] = 1;
    while (n) {
      if (n % 2) W = W.Mul(M).DivMod(C).S;
      n /= 2, M = M.Mul(M).DivMod(C).S;
    }
    ll ret = 0;
    FOR (i, 0, k - 1) ret = (ret + W[i] * a[i]) % P;
    return ret;
  }
};
using Poly_t = Poly<131072 * 2, 998244353, 3>;
template<> decltype(Poly_t::ntt) Poly_t::ntt = {};
