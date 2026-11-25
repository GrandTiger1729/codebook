struct palindromic_tree {
  struct node {
    int next[26], fail, len, diff, slink;
    int cnt, num; // cnt: occurrences, num: # pal. suffixes
    node(int l = 0) : fail(0), len(l), diff(0), slink(0),
      cnt(0), num(0) { FOR (i, 0, 25) next[i] = 0; }
  };
  vector<node> St; // roots 0 (len 0), 1 (len -1)
  vector<char> s;
  vector<pair<int, int>> g; // for best()
  int last, n;
  palindromic_tree() { clear(); }
  void clear() {
    St = {node(0), node(-1)}, St[0].fail = 1;
    s.assign(1, -1), last = 1, n = 0;
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
      int d = St[now].len - St[f].len;
      St[now].fail = f, St[now].diff = d;
      St[now].slink = d == St[f].diff ? St[f].slink : f;
      St[cur].next[c] = now, St[now].num = St[f].num + 1;
    }
    last = St[cur].next[c], ++St[last].cnt;
  }
  // min (dp[j] + 1, j) over palindromes s[j, n), O(log n);
  // call after every add(), dp[0..n-1] already final
  // min palindrome partition: dp[0] = 0, dp[i] = best(dp).F
  pair<int, int> best(vector<int> &dp) {
    g.resize(St.size()); pair<int, int> r(1e9, 0);
    for (int v = last; St[v].len > 0; v = St[v].slink) {
      int j = n - St[St[v].slink].len - St[v].diff;
      g[v] = {dp[j], j};
      if (St[v].diff == St[St[v].fail].diff)
        g[v] = min(g[v], g[St[v].fail]);
      r = min(r, g[v]);
    }
    return {r.F + 1, r.S};
  }
  void count() { // counting cnt
    for (int i = (int)St.size() - 1; i > 1; i--)
      St[St[i].fail].cnt += St[i].cnt;
  }
  int size() { return (int)St.size() - 2; } // #distinct
};
