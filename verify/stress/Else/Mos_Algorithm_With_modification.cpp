#include "prelude.h"
#include "stress.h"
// Problem: array val[0..n-1]; point assignments; queries "distinct values in [l, r]" (0-indexed).
// The template's hooks take arr[idx]; we set arr[i] = i so hooks see indices and read val[] themselves.
const int MAXN = 64;
int blk, arr[MAXN], val[MAXN], n;
int cnt[MAXN], distinct_ = 0, inwin[MAXN], curT = -1;
vector<pair<int, int>> mods; // (pos, value); applying swaps val[pos] <-> mods[t].S
void addv(int v) { if (cnt[v]++ == 0) distinct_++; }
void subv(int v) { if (--cnt[v] == 0) distinct_--; }
void onEvent();
void add(int i) { assert(0 <= i && i < n); inwin[i]++; addv(val[i]); onEvent(); }
void sub(int i) { assert(0 <= i && i < n); inwin[i]--; subv(val[i]); onEvent(); }
void flipTime(int L, int R, int t) {
  auto &[p, v] = mods[t];
  if (L <= p && p <= R) subv(val[p]), addv(v);
  swap(val[p], v);
}
void addTime(int L, int R, int t) { assert(curT == t - 1); flipTime(L, R, t); curT = t; onEvent(); }
void subTime(int L, int R, int t) { assert(curT == t); flipTime(L, R, t); curT = t - 1; onEvent(); }
#include "9_Else/Mos_Algorithm_With_modification.cpp"

// "answer query": the template has no hook there, so we record an answer whenever the live state
// (window as a set of indices, applied-modification count) equals some unanswered query.
struct Q { int l, r, t, want, got = -1; };
vector<Q> qs;
void onEvent() {
  for (auto &q : qs) if (q.got < 0 && q.t == curT) {
    bool eq = true;
    FOR (i, 0, n - 1) if (inwin[i] != (q.l <= i && i <= q.r)) { eq = false; break; }
    if (eq) q.got = distinct_;
  }
}
int main() {
  int cases = 0, nq = 0;
  for (int it = 0; it < 3000; it++) {
    n = rnd(1, 40); int sig = rnd(1, 8);
    FOR (i, 0, n - 1) arr[i] = i, val[i] = rnd(0, sig - 1), inwin[i] = 0;
    fill(cnt, cnt + MAXN, 0); distinct_ = 0; curT = -1;
    mods.clear(); qs.clear();
    vector<int> cur(val, val + n);
    vector<Query> query;
    int ops = rnd(1, 60);
    FOR (k, 1, ops) {
      if (rnd(0, 2) == 0) { int p = rnd(0, n - 1), v = rnd(0, sig - 1); mods.pb(p, v); cur[p] = v; }
      else {
        int l = rnd(0, n - 1), r = rnd(0, n - 1); if (l > r) swap(l, r);
        set<int> s(cur.begin() + l, cur.begin() + r + 1);
        qs.push_back({l, r, (int)mods.size() - 1, (int)s.size()});
      }
    }
    blk = rnd(1, n);
    for (auto &q : qs) query.pb(q.l, q.r, q.t);
    solve(query);
    for (auto &q : qs) assert(q.got == q.want);
    cases++; nq += qs.size();
  }
  printf("Mo+modification ok: %d arrays, %d distinct-count queries (n<=40, random blk)\n", cases, nq);
}
