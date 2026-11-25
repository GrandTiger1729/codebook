struct palindromic_tree {
  struct node {
    int next[26] = {}, fail = 0, len, diff = 0, slink = 0;
    int cnt = 0, num = 0; // occurrences, # pal. suffixes
    node(int l = 0) : len(l) {}
  };
  vector<node> St; // roots 0 (len 0), 1 (len -1)
  vector<char> s;
  int last, n;
  palindromic_tree() { clear(); }
  void clear() {
    St.clear(), s.clear(), last = 1, n = 0;
    St.pb(0), St.pb(-1);
    St[0].fail = 1, s.pb(-1);
  }
  int get_fail(int x) {
    while (s[n - St[x].len - 1] != s[n]) x = St[x].fail;
    return x;
  }
  void add(int c) {
    s.pb(c -= 'a'), ++n;
    int cur = get_fail(last);
    if (!St[cur].next[c]) {
      int now = (int)St.size();
      St.pb(St[cur].len + 2);
      int f = St[get_fail(St[cur].fail)].next[c];
      node &v = St[now];
      v.fail = f, v.num = St[f].num + 1;
      v.diff = v.len - St[f].len; // slink: series link
      v.slink = v.diff == St[f].diff ? St[f].slink : f;
      St[cur].next[c] = now;
    }
    last = St[cur].next[c], ++St[last].cnt;
  }
  int process(string t) { // min palindrome partition
    clear();
    int m = t.size();
    vector<int> dp(m + 1, 1e9), g(m + 2); // dp[prefix len]
    dp[0] = 0;
    FOR (i, 1, m) {
      add(t[i - 1]);
      for (int x = last; x > 1; x = St[x].slink) {
        g[x] = dp[i - St[St[x].slink].len - St[x].diff];
        if (St[x].diff == St[St[x].fail].diff)
          g[x] = min(g[x], g[St[x].fail]);
        dp[i] = min(dp[i], g[x] + 1);
      }
    }
    return dp[m];
  }
  void count() { // counting cnt
    for (int i = (int)St.size() - 1; i > 1; i--)
      St[St[i].fail].cnt += St[i].cnt;
  }
  int size() { return (int)St.size() - 2; } // # distinct
};
