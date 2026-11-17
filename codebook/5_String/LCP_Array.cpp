// sa: suffix array of s (no padding; for sais(),
// drop sa[0]); lcp[i] = LCP of suffixes sa[i], sa[i+1]
vector<int> lcp_array(const auto &s, const auto &sa) {
  int n = s.size(), len = 0;
  vector<int> rnk(n), lcp(max(n - 1, 0));
  FOR (i, 0, n - 1) rnk[sa[i]] = i;
  FOR (i, 0, n - 1) {
    if (rnk[i] == n - 1) {
      len = 0;
      continue;
    }
    int j = sa[rnk[i] + 1];
    while (i + len < n && j + len < n &&
      s[i + len] == s[j + len])
      len++;
    lcp[rnk[i]] = len;
    len = max(0, len - 1);
  }
  return lcp;
}
