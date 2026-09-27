pdd Minimum_Enclosing_Circle(vector<pdd> p, double &r) {
  static mt19937 rng(random_device{}());
  shuffle(ALL(p), rng);
  pdd c = p[0]; r = 0;
  FOR (i, 1, SZ(p) - 1) if (abs(p[i] - c) > r) {
    c = p[i], r = 0;
    FOR (j, 0, i - 1) if (abs(p[j] - c) > r) {
      c = (p[i] + p[j]) / 2, r = abs(p[i] - c);
      FOR (k, 0, j - 1) if (abs(p[k] - c) > r)
        c = circenter(p[i], p[j], p[k]),
        r = abs(p[i] - c);
    }
  }
  return c;
} // expected O(n)
