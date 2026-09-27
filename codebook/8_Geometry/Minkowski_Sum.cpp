vector<pll> Minkowski(vector<pll> A, vector<pll> B) {
  hull(A), hull(B); // neither hull may be degenerate
  int n = SZ(A), m = SZ(B);
  vector<pll> C(1, A[0] + B[0]), s1, s2;
  FOR (i, 0, n - 1) s1.pb(A[(i + 1) % n] - A[i]);
  FOR (i, 0, m - 1) s2.pb(B[(i + 1) % m] - B[i]);
  for (int i = 0, j = 0; i < n || j < m;)
    if (j >= m || (i < n && cross(s1[i], s2[j]) >= 0))
      C.pb(B[j % m] + A[i++]);
    else C.pb(A[i % n] + B[j++]);
  return hull(C), C;
}
