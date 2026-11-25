#include "prelude.h"
// PalTree series links: process() (min palindrome partition) vs O(n^3) brute, and
// CF 906E (Reverses) as a contest driver over diff/slink vs an interval brute.
#include "stress.h"
#include "5_String/PalTree.cpp"
bool isPal(const string &s, int l, int r) { // [l, r)
  for (r--; l < r; l++, r--) if (s[l] != s[r]) return 0;
  return 1;
}
// 906E solution with the template: returns (cost, intervals 1-based in t)
pair<int, vector<pair<int,int>>> solve906(const string &s, const string &t) {
  int n = (int)s.size(); string z;
  FOR (i, 0, n - 1) z += s[i], z += t[i];
  palindromic_tree pal;
  vector<int> dp(2 * n + 1, 1e9), pre(2 * n + 1);
  vector<pair<int, int>> g(2 * n + 3); // (dp value, start)
  dp[0] = 0;
  FOR (i, 1, 2 * n) {
    pal.add(z[i - 1]);
    auto &St = pal.St;
    pair<int, int> r(1e9, 0);
    for (int x = pal.last; x > 1; x = St[x].slink) {
      int j = i - St[St[x].slink].len - St[x].diff;
      g[x] = {dp[j], j};
      if (St[x].diff == St[St[x].fail].diff) g[x] = min(g[x], g[St[x].fail]);
      r = min(r, g[x]);
    }
    if (i % 2) continue;
    if (z[i - 1] == z[i - 2]) dp[i] = dp[i - 2], pre[i] = i - 2;
    if (r.F + 1 < dp[i]) dp[i] = r.F + 1, pre[i] = r.S;
  }
  vector<pair<int,int>> iv;
  if (dp[2 * n] >= 1e9) return {-1, iv};
  for (int i = 2 * n; i > 0; i = pre[i])
    if (i - pre[i] > 2) iv.pb(pre[i] / 2 + 1, i / 2);
  return {dp[2 * n], iv};
}
int main() {
  // 1) plain min palindromic partition vs O(n^3) brute
  FOR (it, 1, 3000) {
    int m = rnd(1, 40), a = rnd(1, 3); string s;
    FOR (i, 1, m) s += char('a' + rnd(0, a - 1));
    palindromic_tree pal; int got = pal.process(s);
    vector<int> b(m + 1, 1e9); b[0] = 0;
    FOR (i, 1, m) FOR (j, 0, i - 1) if (isPal(s, j, i)) b[i] = min(b[i], b[j] + 1);
    assert(got == b[m]);
    pal.count(); // cnt/num still right after process()
    long long occ = 0; FOR (v, 2, (int)pal.St.size() - 1) occ += pal.St[v].cnt;
    long long want = 0; FOR (i, 0, m - 1) FOR (j, i + 1, m) want += isPal(s, i, j);
    assert(occ == want);
  }
  // 2) CF 906E vs brute (interval DP on reversals)
  FOR (it, 1, 3000) {
    int n = rnd(1, 12), a = rnd(1, 3); string s, t;
    FOR (i, 1, n) s += char('a' + rnd(0, a - 1));
    t = s;
    FOR (k, 1, rnd(0, 3)) { int l = rnd(0, n - 1), r = rnd(l, n - 1); reverse(t.begin() + l, t.begin() + r + 1); }
    if (rnd(0, 4) == 0) FOR (i, 0, n - 1) t[i] = char('a' + rnd(0, a - 1));
    vector<int> b(n + 1, 1e9); b[0] = 0;
    FOR (i, 1, n) FOR (j, 0, i - 1) {
      string u = t.substr(j, i - j); reverse(u.begin(), u.end());
      if (u == s.substr(j, i - j)) b[i] = min(b[i], b[j] + (i - j > 1));
    }
    auto [c, iv] = solve906(s, t);
    int want = b[n] >= 1e9 ? -1 : b[n];
    assert(c == want);
    if (c >= 0) {
      assert((int)iv.size() == c);
      string u = t;
      for (auto [l, r] : iv) reverse(u.begin() + l - 1, u.begin() + r);
      assert(u == s);
    }
  }
  // 3) max size: n = 5e5 each, time it
  {
    int n = 500000; string s(n, 'a'), t;
    FOR (i, 0, n - 1) s[i] = 'a' + rnd(0, 1);
    t = s; for (int l = 0; l + 10 <= n; l += 10) reverse(t.begin() + l, t.begin() + l + 7);
    auto st = clock(); auto [c, iv] = solve906(s, t);
    printf("n=5e5 random: cost %d, %.2fs\n", c, double(clock() - st) / CLOCKS_PER_SEC);
    s = string(n, 'a'), t = s; st = clock(); solve906(s, t);
    printf("n=5e5 all a: %.2fs\n", double(clock() - st) / CLOCKS_PER_SEC);
  }
  puts("PalTree series DP: OK");
}
