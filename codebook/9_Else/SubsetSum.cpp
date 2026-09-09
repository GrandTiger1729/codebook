template <size_t MX> // sum(a) < MX
bitset<MX> SubsetSum(const int *a, int n) {
  vector<int> c(MX);
  bitset<MX> dp;
  dp[0] = 1;
  FOR (i, 0, n - 1) ++c[a[i]];
  for (size_t i = 1; i < MX; i++) {
    while (c[i] > 2) c[i] -= 2, ++c[i * 2];
    while (c[i]--) dp |= dp << i;
  }
  return dp;
}
