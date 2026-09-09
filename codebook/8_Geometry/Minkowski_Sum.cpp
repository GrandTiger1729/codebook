vector<pll> Minkowski(vector<pll> A, vector<pll> B) { // |A|,|B|>=3
  hull(A), hull(B);
  vector<pll> C(1, A[0] + B[0]), s1, s2;
  FOR (i, 0, SZ(A) - 1)
    s1.pb(A[(i + 1) % SZ(A)] - A[i]);
  FOR (i, 0, SZ(B) - 1)
    s2.pb(B[(i + 1) % SZ(B)] - B[i]);
  for (int i = 0, j = 0; i < SZ(A) || j < SZ(B);)
    if (j >= SZ(B) || (i < SZ(A) && cross(s1[i], s2[j]) >= 0))
      C.pb(B[j % SZ(B)] + A[i++]);
    else
      C.pb(A[i % SZ(A)] + B[j++]);
  return hull(C), C;
}
