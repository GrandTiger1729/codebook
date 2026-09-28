int FastLinearRecursion(vector<int> a, vector<int> c,
  ll k) {
  // a_n = sigma c_j * a_{n - j - 1}, 0-based
  // O(NlogNlogK), |a| = |c| >= 1
  // Bostan-Mori: a_k = [x^k] P(x) / Q(x)
  int n = SZ(a), m = 1;
  while (m < n * 2 + 1) m <<= 1;
  Poly Q(n + 1);
  Q[0] = 1;
  FOR (i, 0, n - 1) Q[i + 1] = sub(0, c[i]);
  Poly P = Mul(a, Q, n);
  for (; k; k >>= 1) {
    Poly p = P, q = Q;
    p.resize(m), q.resize(m), ntt(p), ntt(q);
    // Q(-x) at w^i is Q(x) at w^(i + m/2)
    FOR (i, 0, m - 1) p[i] = mul(p[i], q[i ^ m / 2]);
    FOR (i, 0, m / 2 - 1)
      q[i] = q[i + m / 2] = mul(q[i], q[i + m / 2]);
    ntt(p, true), ntt(q, true);
    FOR (i, 0, n - 1) P[i] = p[i * 2 + (k & 1)];
    FOR (i, 0, n) Q[i] = q[i * 2];
  }
  return P[0];
}
