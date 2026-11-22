// keep one of OR/AND/XOR; needs MOD, add, sub, mul
void fwt(int *a, int n, bool inv) { // 0 <= a[i] < MOD
  for (int L = 1; L < n; L <<= 1)
    for (int i = 0; i < n; i += L * 2)
      FOR (j, i, i + L - 1) {
        int &x = a[j], &y = a[j + L];
        y = inv ? sub(y, x) : add(y, x); // OR
        x = inv ? sub(x, y) : add(x, y); // AND
        tie(x, y) = pair(add(x, y), sub(x, y)); // XOR
      }
} // XOR inv: then multiply every a[i] by 1/n
const int N = 18; // L <= N, 3 (N + 1) 2^N ints
int f[N + 1][1 << N], g[N + 1][1 << N],
  h[N + 1][1 << N], ct[1 << N];
void subset_convolution(int *a, int *b, int *c, int L) {
  // c_k = \sum_{i | j = k, i & j = 0} a_i * b_j
  int n = 1 << L;
  FOR (i, 1, n - 1) ct[i] = ct[i & (i - 1)] + 1;
  FOR (i, 0, L) fill_n(f[i], n, 0),
    fill_n(g[i], n, 0), fill_n(h[i], n, 0);
  FOR (i, 0, n - 1)
    f[ct[i]][i] = a[i], g[ct[i]][i] = b[i];
  FOR (i, 0, L) fwt(f[i], n, 0), fwt(g[i], n, 0); // OR
  FOR (i, 0, L) FOR (j, 0, i) FOR (x, 0, n - 1)
    h[i][x] = add(h[i][x], mul(f[j][x], g[i - j][x]));
  FOR (i, 0, L) fwt(h[i], n, 1);
  FOR (i, 0, n - 1) c[i] = h[ct[i]][i];
}
