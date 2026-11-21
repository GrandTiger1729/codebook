#define SZ(a) ((int)a.size())
// a_i = sum_{j=0}^{k-1} c_j a_(i-j-1), c: 0-based
// needs 2k terms; MOD prime, add/sub/mul/Pow (see NTT)
vector<int> BerlekampMassey(const vector<int> &s) {
  int n = SZ(s), L = 0, m = 0, b = 1;
  vector<int> C(n + 1), B(n + 1), T;
  C[0] = B[0] = 1;
  FOR (i, 0, n - 1) {
    m++;
    int d = s[i];
    FOR (j, 1, L) d = add(d, mul(C[j], s[i - j]));
    if (!d) continue;
    T = C;
    int coef = mul(d, Pow(b, MOD - 2));
    FOR (j, m, n) C[j] = sub(C[j], mul(coef, B[j - m]));
    if (2 * L > i) continue;
    L = i + 1 - L, B = T, b = d, m = 0;
  }
  vector<int> c(L);
  FOR (j, 0, L - 1) c[j] = sub(0, C[j + 1]);
  return c;
}
