template <typename T> // T: + - * / and == T()
struct matrix { // m variables, n equations
  int n, m;
  T M[MAXN][MAXN + 1], sol[MAXN + 1],
    basis[MAXN][MAXN + 1];
  bool with_basis = true;
  int rank = -1;
  bool fixed[MAXN];
  int solve() { // -1: inconsistent, >= 0: rank of
                // solution space
    FOR (i, 0, n - 1) {
      int piv = 0;
      while (piv < m && M[i][piv] == T()) piv++;
      if (piv == m) continue;
      FOR (j, 0, n - 1) {
        if (i == j) continue;
        T tmp = -M[j][piv] / M[i][piv];
        FOR (k, 0, m)
          M[j][k] = tmp * M[i][k] + M[j][k];
      }
    }
    rank = m;
    FOR (i, 0, n - 1) {
      int piv = 0;
      while (piv < m && M[i][piv] == T()) piv++;
      if (piv == m && !(M[i][m] == T()))
        return rank = -1;
      else if (piv < m) {
        --rank;
        if (with_basis) {
          for (int j = m; j >= piv; j--)
            M[i][j] = M[i][j] / M[i][piv];
          sol[piv] = M[i][m];
          fixed[piv] = true;
        } else {
          sol[piv] = M[i][m] / M[i][piv];
        }
      }
    }
    if (with_basis) {
      FOR (i, 0, n - 1) {
        int piv = 0;
        while (piv < m && M[i][piv] == T()) piv++;
        for (int j = 0, k = 0; j < m; j++) {
          if (!fixed[j]) basis[k++][piv] = -M[i][j];
        }
      }
      for (int j = 0, k = 0; j < m; j++) {
        if (!fixed[j]) basis[k++][j] = 1;
      }
    }
    return rank;
  }
};
