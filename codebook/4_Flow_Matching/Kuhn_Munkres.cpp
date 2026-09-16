struct KM { // 0-base, maximum matching
  ll w[N][N], hl[N], hr[N], slk[N];
  int fl[N], fr[N], pre[N], qu[N], ql, qr, n, m; // n <= m
  bool vl[N], vr[N];
  void init(int _n, int _m) {
    n = _n, m = _m;
    FOR (i, 0, n - 1) fill_n(w[i], m, 0); // perfect matching set to -INF
  }
  void add_edge(int a, int b, ll wei) {
    w[a][b] = max(w[a][b], wei);
  }
  bool Check(int y) {
    if (vr[y] = 1, ~fr[y])
      return vl[qu[qr++] = fr[y]] = 1;
    while (~y) swap(y, fl[fr[y] = pre[y]]);
    return 0;
  }
  void bfs(int s) {
    fill_n(slk, m, INF), fill_n(vl, n, 0), fill_n(vr, m, 0);
    ql = qr = 0, qu[qr++] = s, vl[s] = 1;
    for (ll d;;) {
      while (ql < qr)
        for (int y = 0, x = qu[ql++]; y < m; ++y)
          if (!vr[y] && slk[y] >= (d = hl[x] + hr[y] - w[x][y])) {
            if (pre[y] = x, d) slk[y] = d;
            else if (!Check(y)) return;
        }
      d = INF;
      FOR (y, 0, m - 1)
        if (!vr[y] && d > slk[y]) d = slk[y];
      FOR (y, 0, m - 1) {
        if (vr[y]) hr[y] += d;
        else slk[y] -= d;
      }
      FOR (x, 0, n - 1) if (vl[x]) hl[x] -= d;
      FOR (y, 0, m - 1)
        if (!vr[y] && !slk[y] && !Check(y)) return;
    }
  }
  ll solve() {
    fill_n(fl, n, -1), fill_n(fr, m, -1), fill_n(hr, m, 0);
    FOR (i, 0, n - 1) hl[i] = *max_element(w[i], w[i] + m);
    FOR (i, 0, n - 1) bfs(i);
    ll res = 0;
    FOR (i, 0, n - 1) if (fl[i] != -1) res += w[i][fl[i]];
    return res;
  }
};
