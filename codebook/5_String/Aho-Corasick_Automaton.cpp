struct AC { // remember to build_fail!!!
  int ch[N][C], to[N][C], fail[N], cnt[N], _id;
  vector<int> ord; // BFS order, without the root
  void reset() { _id = 0, newnode(); ord.clear(); }
  AC() { reset(); }
  int newnode() { // root at 0
    fill_n(ch[_id], C, 0), fill_n(to[_id], C, 0);
    fail[_id] = cnt[_id] = 0;
    return _id++;
  }
  int insert(string s) {
    int now = 0;
    for (char c : s) {
      if (!ch[now][c - 'a'])
        ch[now][c - 'a'] = newnode();
      now = ch[now][c - 'a'];
    }
    cnt[now]++; return now;
  }
  void build_fail() { // fail link tree: fail[i] -> i
    queue<int> q;
    FOR (i, 0, C - 1) if (ch[0][i])
      q.push(ch[0][i]), to[0][i] = ch[0][i];
    while (!q.empty()) {
      int v = q.front();
      q.pop(), ord.pb(v);
      FOR (i, 0, C - 1) {
        if (!ch[v][i]) to[v][i] = to[fail[v]][i];
        else {
          int u = ch[v][i], k = fail[v];
          while (k && !ch[k][i]) k = fail[k];
          if (ch[k][i]) k = ch[k][i];
          fail[u] = k, cnt[u] += cnt[k], to[v][i] = u;
          q.push(u);
        }
      }
    }
  }
  // total matches in s: walk to[], sum up cnt[now]
  // occ[v]: times the string of node v occurs in s
  // for a pattern, v is what insert() returned
  vector<int> count(string &s) {
    vector<int> occ(_id);
    int now = 0;
    for (char c : s) occ[now = to[now][c - 'a']]++;
    for (int i = _id - 2; i >= 0; i--)
      occ[fail[ord[i]]] += occ[ord[i]];
    return occ;
  }
} ac;
