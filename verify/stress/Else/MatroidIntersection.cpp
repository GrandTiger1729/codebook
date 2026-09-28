#include "prelude.h"
#include "stress.h"
#include "9_Else/MatroidIntersection.cpp"
// Graphic matroid (edges of a random multigraph with self-loops, n <= 7)
// intersected with a partition matroid (colour c, at most cap[c] edges),
// m <= 12 edges. Size must equal the 2^m brute-force maximum, and the
// returned set must be a forest respecting every colour cap.
int main() {
  int cases = 0, tot = 0;
  FOR (it, 1, 3000) {
    int n = rnd(1, 5) == 1 ? rnd(1, 2) : rnd(3, 7), m = rnd(0, 3) ? rnd(4, 12) : rnd(0, 3), k = rnd(1, 4);
    vector<pair<int, int>> e(m);
    vector<int> col(m), cap(k);
    for (auto &[a, b] : e) {                   // ~1/10 self-loops
      a = rnd(0, n - 1), b = rnd(0, n - 1);
      while (n > 1 && a == b && rnd(1, 10) > 1) b = rnd(0, n - 1);
    }
    for (int &c : col) c = rnd(0, k - 1);
    for (int &c : cap) c = rnd(1, 8) == 1 ? 0 : rnd(1, 4);
    auto ok = [&](const vector<int> &s) {
      vector<int> f(n), cnt(k, 0);
      iota(f.begin(), f.end(), 0);
      function<int(int)> fd = [&](int a) { return f[a] == a ? a : f[a] = fd(f[a]); };
      FOR (i, 0, m - 1) if (s[i]) {
        if (++cnt[col[i]] > cap[col[i]]) return false;
        int a = fd(e[i].F), b = fd(e[i].S);
        if (a == b) return false;
        f[a] = b;
      }
      return true;
    };
    int want = 0;
    FOR (msk, 0, (1 << m) - 1) {
      int pc = __builtin_popcount(msk);
      if (pc <= want) continue;
      vector<int> s(m);
      FOR (i, 0, m - 1) s[i] = msk >> i & 1;
      if (ok(s)) want = pc;
    }
    GraphMat g(n, e);
    ColorMat c(col, cap);
    vector<int> sel = matroid_isect(m, g, c);
    assert((int)sel.size() == m);
    int got = 0;
    for (int x : sel) assert(x == 0 || x == 1), got += x;
    assert(ok(sel));
    assert(got == want);
    tot += got, cases++;
  }
  printf("MatroidIntersection: %d instances (graphic x colour, n<=7, m<=12, avg answer %.2f) vs 2^m brute force\n",
    cases, (double)tot / cases);
}
