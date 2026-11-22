struct palindromic_tree {
  struct node {
    int next[26], fail, len;
    int cnt, num; // cnt: appear times, num: number of
                  // pal. suf.
    node(int l = 0) : fail(0), len(l), cnt(0), num(0) {
      FOR (i, 0, 25) next[i] = 0;
    }
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
      St[now].fail = St[get_fail(St[cur].fail)].next[c];
      St[cur].next[c] = now;
      St[now].num = St[St[now].fail].num + 1;
    }
    last = St[cur].next[c], ++St[last].cnt;
  }
  void count() { // counting cnt
    for (int i = (int)St.size() - 1; i > 1; i--)
      St[St[i].fail].cnt += St[i].cnt;
  }
  int size() { // The number of diff. pal.
    return (int)St.size() - 2;
  }
};
