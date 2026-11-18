struct LCT { // 1-base, node 0 is nil; needs global N
  int ch[N][2], fa[N], rev[N], sz[N]; // path sum;
  ll val[N], sum[N];                  // + -> ^ for xor
  void init(int n) {
    FOR (i, 0, n) {
      ch[i][0] = ch[i][1] = fa[i] = rev[i] = 0;
      val[i] = sum[i] = 0, sz[i] = i > 0;
    }
  }
  bool isr(int x)
  { return ch[fa[x]][0] != x && ch[fa[x]][1] != x; }
  int dir(int x) { return ch[fa[x]][1] == x; }
  void pull(int x) {
    sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + 1;
    sum[x] = sum[ch[x][0]] + sum[ch[x][1]] + val[x];
  }
  void flip(int x)
  { if (x) swap(ch[x][0], ch[x][1]), rev[x] ^= 1; }
  void push(int x) {
    if (rev[x]) flip(ch[x][0]), flip(ch[x][1]);
    rev[x] = 0;
  }
  void rotate(int x) {
    int p = fa[x], g = fa[p], d = dir(x), c = ch[x][!d];
    if (!isr(p)) ch[g][dir(p)] = x;
    fa[x] = g, ch[p][d] = c;
    if (c) fa[c] = p;
    ch[x][!d] = p, fa[p] = x;
    pull(p), pull(x);
  }
  void splay(int x) {
    static int stk[N];
    int top = 0;
    for (int q = x;; q = fa[q]) {
      stk[top++] = q;
      if (isr(q)) break;
    }
    while (top) push(stk[--top]);
    for (; !isr(x); rotate(x))
      if (!isr(fa[x]))
        rotate(dir(x) == dir(fa[x]) ? fa[x] : x);
  }
  int access(int x) { // returns the last splay root
    int q = 0;
    for (; x; q = x, x = fa[x])
      splay(x), ch[x][1] = q, pull(x);
    return q;
  }
  void chroot(int x) { access(x), splay(x), flip(x); }
  void split(int x, int y) // y: root of the x-y path
  { chroot(x), access(y), splay(y); }
  int get_root(int x) {
    access(x), splay(x);
    while (ch[x][0]) push(x), x = ch[x][0];
    return splay(x), x;
  }
  bool conn(int x, int y)
  { return get_root(x) == get_root(y); }
  void link(int x, int y) // x, y not connected
  { chroot(x), fa[x] = y; }
  void cut(int x, int y) { // no-op unless an edge
    split(x, y);
    if (ch[y][0] == x && sz[x] == 1)
      ch[y][0] = fa[x] = 0, pull(y);
  }
  int lca(int x, int y) // under the current root
  { return access(x), access(y); }
  void change(int x, ll v)
  { splay(x), val[x] = v, pull(x); }
  ll query(int x, int y) { return split(x, y), sum[y]; }
};
