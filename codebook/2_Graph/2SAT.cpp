// 0-base, reuses SCC, so it needs the same adj.
// ~x is NOT x; literal a lives on node en(a) + 1
struct TwoSat {
  int n;
  vector<int> ans; // 0 / 1, set by solve()
  TwoSat(int n = 0) : n(n) {}
  int en(int a) { return max(a * 2, -1 - a * 2); }
  int add_var() { return n++; } // adj must have room
  void either(int a, int b) { // a or b
    int x = en(a), y = en(b);
    adj[(x ^ 1) + 1].pb(y + 1);
    adj[(y ^ 1) + 1].pb(x + 1);
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
  bool solve() { // scc[] is numbered sink first
    SCC scc(n * 2);
    scc.work();
    ans.assign(n, 0);
    FOR (i, 0, n - 1) {
      int a = i * 2 + 1, b = i * 2 + 2;
      if (scc.scc[a] == scc.scc[b]) return false;
      ans[i] = scc.scc[a] < scc.scc[b];
    }
    return true;
  }
};
