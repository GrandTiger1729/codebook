struct matrix { // m variables, n equations
  int n, m;
  fraction M[MAXN][MAXN + 1], sol[MAXN], basis[MAXN][MAXN];
	bool with_basis = true;
  int rank = -1;
  bool fixed[MAXN];
  int solve() { // -1: inconsistent, >= 0: rank of solution space
    for (int i = 0; i < n; ++i) {
      int piv = 0;
      while (piv < m && !M[i][piv].n) ++piv;
      if (piv == m) continue;
      for (int j = 0; j < n; ++j) {
        if (i == j) continue;
        fraction tmp = -M[j][piv] / M[i][piv];
        for (int k = 0; k <= m; ++k)
          M[j][k] = tmp * M[i][k] + M[j][k];
      }
    }
    rank = m;
    for (int i = 0; i < n; ++i) {
      int piv = 0;
      while (piv < m && !M[i][piv].n) ++piv;
      if (piv == m && M[i][m].n) return rank = -1;
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
      for (int i = 0; i < n; ++i) { 
				int piv = 0;
				while (piv < m && !M[i][piv].n) ++piv;
				for (int j = 0, k = 0; j < m; ++j) {
					if (!fixed[j])
						basis[k++][piv] = -M[i][j];
				}
			}
			for (int j = 0, k = 0; j < m; ++j) {
				if (!fixed[j])
					basis[k++][j] = 1;
			}
    }
    return rank;
  }
};