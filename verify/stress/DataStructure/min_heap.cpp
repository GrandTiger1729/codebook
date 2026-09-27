#include "prelude.h"
#include "stress.h"
#include "3_Data_Structure/min_heap.cpp"
// K heaps, random push / pop / top / join / add_lazy vs a multiset per heap
// holding the true values; checks the leftist invariant (right spine is short)
typedef min_heap<ll, int> H;
int spine(H::node *x) { int s = 0; for (; x; x = x->r) s++; return s; }
int check(H::node *x) { // returns size; dist = right-spine length, dist(l) >= dist(r)
  if (!x) return 0;
  int dl = x->l ? x->l->dist : 0, dr = x->r ? x->r->dist : 0;
  assert(dl >= dr && x->dist == dr + 1);
  return 1 + check(x->l) + check(x->r);
}
int main() {
  int ops = 0;
  FOR (it, 1, 3000) {
    int K = rnd(1, 6); vector<H> h(K); vector<multiset<pair<ll, int>>> ref(K);
    int id = 0, T = rnd(1, 300);
    FOR (t, 1, T) {
      int a = rnd(0, K - 1), op = rnd(0, 9);
      if (op < 4) { ll w = rnd(-20, 20); h[a].push({w, id}); ref[a].insert({w, id++}); }
      else if (op < 6) {
        assert(h[a].empty() == ref[a].empty());
        if (!ref[a].empty()) {
          auto [w, v] = h[a].top();
          assert(w == ref[a].begin()->F);                 // min key
          assert(ref[a].count({w, v}));                   // an element with that key
          h[a].pop(); ref[a].erase(ref[a].find({w, v}));
        }
      } else if (op < 8) {
        int b = rnd(0, K - 1); if (a == b) continue;
        h[a].join(h[b]); ref[a].insert(ref[b].begin(), ref[b].end()); ref[b].clear();
        assert(h[b].empty());
      } else {
        ll v = rnd(-10, 10); h[a].add_lazy(v);
        multiset<pair<ll, int>> s; for (auto [w, i] : ref[a]) s.insert({w + v, i});
        ref[a] = s;
      }
      ops++;
    }
    FOR (a, 0, K - 1) {                                   // drain: exact multiset of keys
      assert(check(h[a].root) == (int)ref[a].size());
      vector<ll> x, y;
      while (!h[a].empty()) x.pb(h[a].top().F), h[a].pop();
      for (auto [w, i] : ref[a]) y.pb(w);
      assert(x == y);
    }
  }
  H big; int n = 200000;                                  // right spine stays O(log n)
  FOR (i, 1, n) big.push({rnd(-1e9, 1e9), i});
  int sp = spine(big.root); assert(sp <= 18);
  ll prev = LLONG_MIN;
  FOR (i, 1, n) { assert(big.top().F >= prev); prev = big.top().F; big.pop(); }
  printf("min_heap: 3000 cases, %d ops vs multiset; n=2e5 right spine %d, sorted drain OK\n", ops, sp);
}
