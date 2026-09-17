template <typename T> // T: + - * / and == T()
struct matrix { // m variables, n equations
  int n, m, rank, pv[MAXN];
  bool fixed[MAXN];
  T M[MAXN][MAXN + 1], sol[MAXN + 1],
    basis[MAXN][MAXN + 1];
  int solve() { // -1: inconsistent, else dim of
    rank = m; // the solution space
    FOR (i, 0, n - 1) {
      int &p = pv[i] = 0;
      while (p < m && M[i][p] == T()) p++;
      if (p == m) continue;
      FOR (j, 0, n - 1) if (i != j) {
        T t = -M[j][p] / M[i][p];
        FOR (k, 0, m) M[j][k] = t * M[i][k] + M[j][k];
      }
    }
    FOR (i, 0, n - 1) {
      int p = pv[i];
      if (p == m) {
        if (!(M[i][m] == T())) return rank = -1;
        continue;
      }
      rank--;
      for (int j = m; j >= p; j--)
        M[i][j] = M[i][j] / M[i][p];
      sol[p] = M[i][m], fixed[p] = 1;
    }
    FOR (i, 0, n - 1)
      for (int j = 0, k = 0; j < m; j++)
        if (!fixed[j]) basis[k++][pv[i]] = -M[i][j];
    for (int j = 0, k = 0; j < m; j++)
      if (!fixed[j]) basis[k++][j] = 1;
    return rank;
  }
};
