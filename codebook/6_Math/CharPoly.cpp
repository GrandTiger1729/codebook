// det(xI - a) mod prime p, O(n^3); res[k]: coef of x^k
vector<ll> char_poly(vector<vector<ll>> a, ll p) {
  int n = a.size();
  auto inv = [&](ll x) {
    ll r = 1;
    for (ll e = p - 2; e; e >>= 1, x = x * x % p)
      if (e & 1) r = r * x % p;
    return r;
  };
  FOR (i, 0, n - 3) { // similar upper Hessenberg matrix
    int j = i + 1;
    while (j < n && !a[j][i]) j++;
    if (j == n) continue;
    swap(a[i + 1], a[j]);
    FOR (k, 0, n - 1) swap(a[k][i + 1], a[k][j]);
    ll iv = inv(a[i + 1][i]);
    FOR (r, i + 2, n - 1) { // row r -= c row i+1, then
      ll c = a[r][i] * iv % p; // col i+1 += c col r
      FOR (k, i, n - 1)
        a[r][k] = (a[r][k] + (p - c) * a[i + 1][k]) % p;
      FOR (k, 0, n - 1)
        a[k][i + 1] = (a[k][i + 1] + c * a[k][r]) % p;
    }
  }
  // f[i]: char poly of the top-left i x i block
  vector<vector<ll>> f(n + 1, vector<ll>(n + 1));
  f[0][0] = 1;
  FOR (i, 1, n) {
    FOR (k, 1, i) f[i][k] = f[i - 1][k - 1];
    ll prod = 1; // a[j+1][j] * ... * a[i-1][i-2]
    for (int j = i - 1; j >= 0; j--) {
      ll c = prod * a[j][i - 1] % p;
      FOR (k, 0, j)
        f[i][k] = (f[i][k] + (p - c) * f[j][k]) % p;
      if (j) prod = prod * a[j][j - 1] % p;
    }
  }
  return f[n];
}
