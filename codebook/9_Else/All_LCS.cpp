void all_lcs(string s, string t) { // 0-base
  vector<int> h((int)t.size());
  iota(h.begin(), h.end(), 0);
  FOR (a, 0, (int)s.size() - 1) {
    int v = -1;
    FOR (c, 0, (int)t.size() - 1)
      if (s[a] == t[c] || h[c] < v)
        swap(h[c], v);
    // LCS(s[0, a], t[b, c]) = 
    // c - b + 1 - sum([h[i] >= b] | i <= c)
    // h[i] might become -1 !!
  }
}
