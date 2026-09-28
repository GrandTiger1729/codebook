#include "prelude.h"
#include "stress.h"
const int MAXN = 105;
vector<int> adj[MAXN];
#include "2_Graph/SCC.cpp"
#include "2_Graph/2SAT.cpp"
// Random clauses over n <= 10 variables: either(a, b), set_value(a) and
// at_most_one(v) on literals (a or ~a). Satisfiability must match 2^n
// brute force, and when satisfiable, ans[] must satisfy every constraint.
// at_most_one adds auxiliary variables via add_var(); they are ignored.
int main() {
  int cases = 0, sat = 0, amo = 0;
  FOR (it, 1, 20000) {
    int n = rnd(1, 10);
    FOR (i, 0, MAXN - 1) adj[i].clear();
    TwoSat ts(n);
    vector<pair<int, int>> cl;          // either / set_value (b == a)
    vector<vector<int>> am;             // at_most_one groups
    auto lit = [&]() { int a = rnd(0, n - 1); return rnd(0, 1) ? a : ~a; };
    int m = rnd(0, 2 * n), k = rnd(0, 3);
    FOR (i, 1, m) {
      int t = rnd(1, 6);
      if (t == 1) { int a = lit(); ts.set_value(a), cl.pb(a, a); }
      else { int a = lit(), b = lit(); ts.either(a, b), cl.pb(a, b); }
    }
    FOR (j, 1, k) {
      vector<int> vs(n); iota(vs.begin(), vs.end(), 0);
      shuffle(vs.begin(), vs.end(), rng);
      vs.resize(rnd(0, min(n, 6)));     // distinct variables, random signs
      for (int &x : vs) if (rnd(0, 1)) x = ~x;
      ts.at_most_one(vs), am.pb(vs), amo++;
    }
    auto val = [](const vector<int> &x, int a) { return a >= 0 ? x[a] == 1 : x[~a] == 0; };
    auto ok = [&](const vector<int> &x) {
      for (auto [a, b] : cl) if (!val(x, a) && !val(x, b)) return false;
      for (auto &vs : am) {
        int c = 0;
        for (int a : vs) c += val(x, a);
        if (c > 1) return false;
      }
      return true;
    };
    bool want = false;
    FOR (msk, 0, (1 << n) - 1) {
      vector<int> x(n);
      FOR (i, 0, n - 1) x[i] = msk >> i & 1;
      if (ok(x)) { want = true; break; }
    }
    assert(2 * ts.n < MAXN);
    bool got = ts.solve();
    assert(got == want);
    if (got) {
      assert((int)ts.ans.size() == ts.n);
      assert(ok(ts.ans));
      sat++;
    }
    cases++;
  }
  printf("2SAT: %d instances (n<=10, either/set_value/%d at_most_one), %d satisfiable, vs 2^n brute force\n",
    cases, amo, sat);
}
