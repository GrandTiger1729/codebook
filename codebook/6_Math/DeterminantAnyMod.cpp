// det(a) mod any m (Euclid on rows, no inverses);
// entries in [0, m), O(n^3 + n^2 log m)
ll det(vector<vector<ll>> a, ll m) {
  int n = a.size();
  ll res = 1 % m;
  FOR (i, 0, n - 1) {
    FOR (j, i + 1, n - 1) while (a[j][i]) {
      ll q = a[i][i] / a[j][i];
      FOR (k, i, n - 1)
        a[i][k] = (a[i][k] + (m - q) * a[j][k]) % m;
      swap(a[i], a[j]), res = (m - res) % m;
    }
    res = res * a[i][i] % m;
  }
  return res;
}
