struct Maximum_Clique {
  typedef bitset<SIZE> bst;
  bst N[SIZE], empty;
  int p[SIZE], n, ans;
  void BronKerbosch2(bst R, bst P, bst X) {
    if (P == empty && X == empty)
      return ans = max(ans, (int)R.count()), void();
    bst tmp = P | X;
    int u;
    if ((R | P | X).count() <= ans) return;
    FOR (uu, 0, n - 1) {
      u = p[uu];
      if (tmp[u] == 1) break;
    }
    // if (double(clock())/CLOCKS_PER_SEC > .999)
    // return;
    bst now2 = P & ~N[u];
    FOR (vv, 0, n - 1) {
      int v = p[vv];
      if (now2[v] == 1) {
        R[v] = 1;
        BronKerbosch2(R, P & N[v], X & N[v]);
        R[v] = 0, P[v] = 0, X[v] = 1;
      }
    }
  }
  void init(int _n) {
    n = _n;
    FOR (i, 0, n - 1) N[i].reset();
  }
  void add_edge(int u, int v) {
    N[u][v] = N[v][u] = 1;
  }
  int solve() { // remember srand
    bst R, P, X;
    ans = 0, P.flip();
    FOR (i, 0, n - 1) p[i] = i;
    shuffle(p, p + n, rng), BronKerbosch2(R, P, X);
    return ans;
  }
} G;
