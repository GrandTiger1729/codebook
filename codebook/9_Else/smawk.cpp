// For every 2x2 submatrix (row 0 above row 1):
// If M[0][0] < M[0][1], M[1][0] < M[1][1]
// If M[0][0] == M[0][1], M[1][0] <= M[1][1]
// M[i][ans_i] is the best value in the i-th row
// select, e.g. row minima (<=, not <):
// [&](int r, int u, int v) { return f(r, v) <= f(r, u); }
vector<int> smawk(int N, int M, auto &&select) {
  auto dc = [&](auto self, const vector<int> &r,
    const vector<int> &c) {
    if (r.empty()) return vector<int>{};
    const int n = r.size(); vector<int> ans(n), nr, nc;
    for (int i : c) {
      while (!nc.empty() &&
          select(r[nc.size() - 1], nc.back(), i))
        nc.pop_back();
      if (int(nc.size()) < n) nc.pb(i);
    }
    for (int i = 1; i < n; i += 2) nr.pb(r[i]);
    const auto na = self(self, nr, nc);
    for (int i = 1; i < n; i += 2) ans[i] = na[i >> 1];
    for (int i = 0, j = 0; i < n; i += 2) {
      ans[i] = nc[j];
      const int end = i + 1 == n ? nc.back() : ans[i + 1];
      while (nc[j] != end)
        if (select(r[i], ans[i], nc[++j])) ans[i] = nc[j];
    }
    return ans;
  };
  vector<int> R(N), C(M);
  iota(R.begin(), R.end(), 0);
  iota(C.begin(), C.end(), 0);
  return dc(dc, R, C);
}
