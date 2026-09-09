struct Matrix {
  int n, m;
  ll M[MAXN][MAXN];
  int row_swap(int i, int j) {
    if (i == j) return 0;
    FOR (k, 0, m - 1) swap(M[i][k], M[j][k]);
    return 1;
  }
  ll det() { // return the number of swaps
    int rt = 0;
    FOR (i, 0, n - 1) {
      int piv = i;
      while (piv < n && !M[piv][i]) ++piv;
      if (piv == n) continue;
      rt += row_swap(i, piv);
      FOR (j, i + 1, n - 1) {
        while (M[j][i]) {
          int tmp = P - M[i][i] / M[j][i];
          FOR (k, i, m - 1)
            M[i][k] = (M[j][k] * tmp + M[i][k]) % P;
          rt += row_swap(i, j);
        }
      }
    }
    rt = (rt & 1) ? P - 1 : 1;
    FOR (i, 0, n - 1) rt = rt * M[i][i] % P;
    return rt; 
    // round(rt) if using double to cal. int. det
  }
};
