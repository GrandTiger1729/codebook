#include "prelude.h"
#include "stress.h"
const ll INF = 4e18; // the template needs an external INF (returned as -INF when empty)
#include "3_Data_Structure/LiChaoST.cpp"

int main() {
  ll cases = 0, queries = 0;
  FOR (it, 1, 3000) {
    int n = it <= 1500 ? rnd(1, 20) : rnd(1, 1000);
    ll C = rnd(0, 1) ? 5 : 1e9; // small coefficients: many ties
    LiChao lc(n);
    vector<L> lines;
    int ops = rnd(1, 300);
    FOR (op, 1, ops) {
      if (rnd(0, 1)) {
        L ln(rnd(-C, C), rnd(-C * 1000, C * 1000), lines.size());
        lc.insert(ln), lines.pb(ln);
      } else {
        ll x = rnd(0, n - 1), want = -INF;
        for (auto &ln : lines) want = max(want, ln.at(x));
        assert(lc.query(x) == want), queries++;
      }
    }
    FOR (x, 0, n - 1) { // full sweep at the end
      ll want = -INF;
      for (auto &ln : lines) want = max(want, ln.at(x));
      assert(lc.query(x) == want), queries++;
    }
    cases++;
  }
  printf("LiChaoST: %lld cases, %lld queries OK\n", cases, queries);
}
