// min k >= 0 with x^k = y (mod m), or -1
int DiscreteLog(ll x, ll y, int m) {
  int n = sqrt(m) + 1;
  ll s = 1 % m;
  x %= m, y %= m;
  FOR (k, 0, n - 1) {
    if (s == y) return k;
    s = s * x % m;
  }
  if (__gcd(s, (ll)m) != __gcd(y, (ll)m)) return -1;
  unordered_map<ll, int> baby; // y x^j -> largest j
  FOR (j, 1, n) baby[y = y * x % m] = j;
  ll g = s;
  FOR (i, 2, n + 1) {
    g = g * s % m;
    if (baby.count(g)) return 1LL * i * n - baby[g];
  }
  return -1;
}
