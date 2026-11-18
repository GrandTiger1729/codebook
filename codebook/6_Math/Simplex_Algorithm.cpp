struct simplex { // max cx s.t. Ax <= b, x >= 0
  int n, m, B[MAXN], N[MAXM + 1]; // n rows, m vars
  double A[MAXN + 2][MAXM + 2], b[MAXN], c[MAXM], x[MAXM];
  void pivot(int r, int s) {
    double inv = 1 / A[r][s];
    FOR (i, 0, n + 1) if (i != r) {
      double t = -A[i][s] * inv;
      if (abs(A[i][s]) > 1e-9) FOR (j, 0, m + 1)
        if (j != s) A[i][j] += A[r][j] * t;
      A[i][s] = t;
    }
    FOR (j, 0, m + 1) if (j != s) A[r][j] *= inv;
    A[r][s] = inv;
    swap(B[r], N[s]);
  }
  bool lt(double *a, int j, int s, bool bl = 0) {
    auto k = [&](int i) {
      return pair(bl ? -(a[i] < -1e-9) : a[i], N[i]);
    };
    return s < 0 || k(j) < k(s);
  }
  bool run(int phase) { // 0: unbounded
    for (int z = n + phase - 1, bl = 0; ; ) {
      int s = -1, r = -1;
      FOR (j, 0, m)
        if (N[j] != -phase && lt(A[z], j, s, bl)) s = j;
      if (A[z][s] >= -1e-9) return 1;
      FOR (i, 0, n - 1) if (A[i][s] > 1e-9 && (r < 0 ||
          pair(A[i][m + 1] / A[i][s], B[i]) <
          pair(A[r][m + 1] / A[r][s], B[r]))) r = i;
      if (r < 0) return 0;
      bl = A[r][m + 1] < 1e-9; // stuck: Bland's rule
      pivot(r, s);
    }
  }
  double solve() { // -INF: infeasible
    fill_n(A[n], m + 2, 0), fill_n(A[n + 1], m + 2, 0);
    FOR (i, 0, n - 1)
      B[i] = m + i, A[i][m] = -1, A[i][m + 1] = b[i];
    FOR (j, 0, m - 1) N[j] = j, A[n][j] = -c[j];
    N[m] = -1, A[n + 1][m] = 1;
    int r = 0;
    FOR (i, 1, n - 1) if (b[i] < b[r]) r = i;
    if (A[r][m + 1] < -1e-9) {
      pivot(r, m);
      if (!run(2) || A[n + 1][m + 1] < -1e-9)
        return -INF;
      FOR (i, 0, n - 1) if (B[i] == -1) {
        int s = 0;
        FOR (j, 1, m) if (lt(A[i], j, s)) s = j;
        pivot(i, s);
      }
    }
    bool ok = run(1);
    fill_n(x, m, 0);
    FOR (i, 0, n - 1) if (B[i] < m) x[B[i]] = A[i][m + 1];
    return ok ? A[n][m + 1] : INF;
  }
};
