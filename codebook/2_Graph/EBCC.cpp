struct EBCC { // need adj
  int n, dfcnt = 0, bccnt = 0;
  vector<int> dfn, low, bcc, st;
  EBCC() {}
  EBCC(int n) : n(n) { dfn = low = bcc = vector<int>(n + 1, 0); }
  void tarjan(int pos, int fa) {
    dfn[pos] = low[pos] = ++dfcnt;
    st.emplace_back(pos);
    bool vs = false;
    for (int np : adj[pos]) {
      if (np != fa || vs == true) {
        if (dfn[np] == 0) {
          tarjan(np, pos);
          low[pos] = min(low[pos], low[np]);
          if (dfn[pos] < low[np]) {
            bccnt++;
            while (1) {
              int x = st.back();
              bcc[x] = bccnt;
              st.pop_back();
              if (x == np) {
                break;
              }
            }
          }
        } else {
          low[pos] = min(low[pos], dfn[np]);
        }
      } else {
        vs = true;
      }
    }
    if (pos == fa) {
      bccnt++;
      while (st.size()) {
        bcc[st.back()] = bccnt;
        st.pop_back();
      }
    }
  }
  void work() {
    for (int i = 1; i <= n; i++) {
      if (dfn[i] == 0) {
        tarjan(i, i);
      }
    }
  }
};
