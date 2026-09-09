// 0-base, need global N >= 2 * #vars; ~x means NOT x
struct TwoSat {
  int n, dft;
  vector<int> g[N], st;
  int val[N], comp[N], ans[N];
  void init(int _n) {
    n = _n, dft = 0, st.clear();
    FOR (i, 0, n * 2 - 1) g[i].clear();
  }
  int add_var() { // at_most_one needs |v| - 2 of these
    g[n * 2].clear(), g[n * 2 + 1].clear();
    return n++;
  }
  void either(int a, int b) { // a or b
    a = max(a * 2, -1 - a * 2);
    b = max(b * 2, -1 - b * 2);
    g[a].pb(b ^ 1), g[b].pb(a ^ 1);
  }
  void set_value(int x) { either(x, x); }
  void at_most_one(const vector<int> &v) {
    if (v.size() <= 1) return;
    int cur = ~v[0];
    FOR (i, 2, (int)v.size() - 1) {
      int nxt = add_var();
      either(cur, ~v[i]), either(cur, nxt);
      either(~v[i], nxt), cur = ~nxt;
    }
    either(cur, ~v[1]);
  }
  int dfs(int x) { // tarjan, reverse topo order
    int low = val[x] = ++dft, y;
    st.pb(x);
    for (int e : g[x])
      if (!comp[e]) {
        if (!val[e]) dfs(e);
        low = min(low, val[e]);
      }
    if (low == val[x]) do {
        y = st.back(), st.pop_back(), comp[y] = low;
        if (ans[y >> 1] == -1) ans[y >> 1] = y & 1;
      } while (y != x);
    return val[x] = low;
  }
  bool solve() { // ans[i] = 0 / 1
    fill_n(ans, n, -1);
    fill_n(val, n * 2, 0), fill_n(comp, n * 2, 0);
    FOR (i, 0, n * 2 - 1)
      if (!comp[i]) dfs(i);
    FOR (i, 0, n - 1)
      if (comp[i * 2] == comp[i * 2 + 1]) return false;
    return true;
  }
};
