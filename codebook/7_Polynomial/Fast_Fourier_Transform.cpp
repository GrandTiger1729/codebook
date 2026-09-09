typedef complex<double> cplx;
// safe if (sum a_i^2 + sum b_i^2) log2(n) < 9e14
void fft(vector<cplx> &a) {
  int n = a.size(), L = 31 - __builtin_clz(n);
  static vector<complex<long double>> R(2, 1);
  static vector<cplx> rt(2, 1);
  for (static int k = 2; k < n; k *= 2) {
    R.resize(n), rt.resize(n);
    auto x = polar(1.0L, acos(-1.0L) / k);
    FOR (i, k, 2 * k - 1)
      rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
  }
  vector<int> rev(n);
  FOR (i, 0, n - 1)
    rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
  FOR (i, 0, n - 1)
    if (i < rev[i]) swap(a[i], a[rev[i]]);
  for (int k = 1; k < n; k *= 2)
    for (int i = 0; i < n; i += 2 * k)
      FOR (j, 0, k - 1) {
        cplx z = rt[j + k] * a[i + j + k];
        a[i + j + k] = a[i + j] - z;
        a[i + j] += z;
      }
}
vector<double> conv(
  const vector<double> &a, const vector<double> &b) {
  if (a.empty() || b.empty()) return {};
  int m = a.size() + b.size() - 1;
  int L = 32 - __builtin_clz(m), n = 1 << L;
  vector<double> res(m);
  vector<cplx> in(n), out(n);
  copy(a.begin(), a.end(), in.begin());
  FOR (i, 0, (int)b.size() - 1) in[i].imag(b[i]);
  fft(in);
  for (cplx &x : in) x *= x;
  FOR (i, 0, n - 1)
    out[i] = in[-i & (n - 1)] - conj(in[i]);
  fft(out);
  FOR (i, 0, m - 1) res[i] = imag(out[i]) / (4 * n);
  return res;
}
