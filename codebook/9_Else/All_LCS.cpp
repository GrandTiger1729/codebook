struct AllLCS { // 0-base
  string t;
  vector<int> h;
  AllLCS(const string &_t) : t(_t), h(t.size()) {
    iota(h.begin(), h.end(), 0);
  }
  void push(char c) { // append c to the other string
    int v = -1;
    FOR (i, 0, (int)t.size() - 1)
      if (t[i] == c || h[i] < v) swap(h[i], v);
  }
  // after pushing s[0, a], for every b <= c:
  // LCS(s[0, a], t[b, c]) =
  // c - b + 1 - #{i <= c : h[i] >= b}
  // h[i] might become -1 !!
};
