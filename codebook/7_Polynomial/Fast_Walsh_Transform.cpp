/* x: a[j], y: a[j + (L >> 1)]
or: (y += x * op), and: (x += y * op)
xor: (x, y = (x + y) * op, (x - y) * op)
invop: or, and, xor = -1, -1, 1/2 */
void fwt(int *a, int n, int op) { //or
  for (int L = 2; L <= n; L <<= 1)
    for (int i = 0; i < n; i += L)
      FOR (j, i, j + (L >> 1) - 1)
        a[j + (L >> 1)] += a[j] * op;
}
const int N = 21;
int f[N][1 << N], g[N][1 << N], h[N][1 << N], ct[1 << N];
void subset_convolution(int *a, int *b, int *c, int L) {
  // c_k = \sum_{i | j = k, i & j = 0} a_i * b_j
  int n = 1 << L;
  FOR (i, 1, n - 1)
    ct[i] = ct[i & (i - 1)] + 1;
  FOR (i, 0, n - 1)
    f[ct[i]][i] = a[i], g[ct[i]][i] = b[i];
  FOR (i, 0, L)
    fwt(f[i], n, 1), fwt(g[i], n, 1);
  FOR (i, 0, L) FOR (j, 0, i) FOR (x, 0, n - 1)
    h[i][x] += f[j][x] * g[i - j][x];
  FOR (i, 0, L)
    fwt(h[i], n, -1);
  FOR (i, 0, n - 1)
    c[i] = h[ct[i]][i];
}
