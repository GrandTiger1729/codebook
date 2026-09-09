int DiscreteLog(int s, int x, int y, int m) {
  int kStep = sqrt(m) + 1;
  unordered_map<int, int> p;
  int b = 1;
  FOR (i, 0, kStep - 1) {
    p[y] = i;
    y = 1LL * y * x % m;
    b = 1LL * b * x % m;
  }
  for (ll i = 0; i <= m; i += kStep) {
    s = 1LL * s * b % m;
    if (p.find(s) != p.end()) return i + kStep - p[s];
  }
  return -1;
}
int DiscreteLog(int x, int y, int m) {
  if (m == 1) return 0;
  int s = 1;
  FOR (i, 0, 99) {
    if (s == y) return i;
    s = 1LL * s * x % m;
  }
  if (s == y) return 100;
  int p = 100 + DiscreteLog(s, x, y, m);
  if (fpow(x, p, m) != y) return -1;
  return p;
} // find minimum k such that x^k == y (mod m)
