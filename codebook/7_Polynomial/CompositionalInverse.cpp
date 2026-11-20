Poly CompInv(Poly f) {
  // O(Nlog^2N), f[0] = 0, f[1] != 0, return g: f(g(x)) = x
  int n = SZ(f);
  if (n == 1) return {0};
  Poly w(n), h(n - 1), g(n);
  w[n - 1] = 1;
  Poly a = power_proj(w, f, n - 1); // a[i] = [x^(n-1)] f^i
  FOR (i, 1, n - 1)
    h[n - 1 - i] = mul(a[i], Pow(i, mod - 2));
  int ih0 = Pow(h[0], mod - 2), g1 = Pow(f[1], mod - 2);
  FOR (i, 0, n - 2) h[i] = mul(h[i], ih0);
  // h = (x/(f[1]g))^(n-1), h[0] = 1: PolyPow takes k mod p
  h = PolyPow(h, sub(0, Pow(n - 1, mod - 2)));
  FOR (i, 0, n - 2) g[i + 1] = mul(h[i], g1);
  return g;
}
