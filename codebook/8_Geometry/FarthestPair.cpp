// C: hull() output (CCW, no three collinear); returns
// indices of a farthest pair (rotating calipers), O(n)
pii farthest_pair(const vector<pll> &C) {
  int n = SZ(C), j = 1 % n;
  pii res(0, 0);
  FOR (i, 0, n - 1) {
    pll e = C[(i + 1) % n] - C[i];
    while (cross(e, C[(j + 1) % n] - C[i]) >
      cross(e, C[j] - C[i])) j = (j + 1) % n;
    for (int k : {i, (i + 1) % n})
      if (abs2(C[j] - C[k]) > abs2(C[res.F] - C[res.S]))
        res = pii(k, j);
  }
  return res;
}
