// needs FFT; ok while n log2(n) * M < 8.6e14
// inputs must already be in [0, M)
template <int M>
vector<ll> conv_mod(
  const vector<ll> &a, const vector<ll> &b) {
  if (a.empty() || b.empty()) return {};
  int m = a.size() + b.size() - 1;
  int B = 32 - __builtin_clz(m), n = 1 << B;
  int cut = (int)sqrt(M);
  vector<ll> res(m);
  vector<cplx> L(n), R(n), os(n), ol(n);
  FOR (i, 0, (int)a.size() - 1)
    L[i] = cplx((int)a[i] / cut, (int)a[i] % cut);
  FOR (i, 0, (int)b.size() - 1)
    R[i] = cplx((int)b[i] / cut, (int)b[i] % cut);
  fft(L), fft(R);
  FOR (i, 0, n - 1) {
    int j = -i & (n - 1);
    ol[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
    os[j] =
      (L[i] - conj(L[j])) * R[i] / (2.0 * n) / 1i;
  }
  fft(ol), fft(os);
  FOR (i, 0, m - 1) {
    ll av = (ll)(real(ol[i]) + .5);
    ll cv = (ll)(imag(os[i]) + .5);
    ll bv =
      (ll)(imag(ol[i]) + .5) + (ll)(real(os[i]) + .5);
    res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
  }
  return res;
}
