#include "prelude.h"
#include "stress.h"
#include "9_Else/cyc_tsearch.cpp"

// f = cyclic shift of a strictly-decreasing-then-strictly-increasing sequence, pred(a,b) = f(a) < f(b).
// Answer must be an index whose value is the global minimum.
// Variant "flat": minimum may be a plateau of up to 2 equal adjacent values (tangent-through-collinear case).
int main() {
  int cases = 0, flatcases = 0;
  for (int it = 0; it < 100000; it++) {
    int n = rnd(1, it % 100 == 0 ? 200 : 12);
    vector<ll> vals(n); for (auto &x : vals) x = rnd(-1000000, 1000000);
    sort(vals.begin(), vals.end()); vals.erase(unique(vals.begin(), vals.end()), vals.end());
    n = vals.size();
    // build unimodal: vals[0] is min; others split to left (decreasing) / right (increasing)
    vector<ll> L, R;
    FOR (i, 1, n - 1) (rnd(0, 1) ? L : R).pb(vals[i]);
    sort(L.rbegin(), L.rend()); sort(R.begin(), R.end());
    vector<ll> f = L; f.pb(vals[0]);
    bool flat = it % 2 && n >= 3;
    if (flat) f.pb(vals[0]);
    for (ll x : R) f.pb(x);
    if (flat && f.size() > (size_t)n) { f.pop_back(); }  // keep f non-empty, drop one to vary size
    int m = f.size();
    if (flat) { // max must also be unique-ish: fine as long as only the min is flat
      int cntmin = count(f.begin(), f.end(), vals[0]); if (cntmin != 2) flat = false;
    }
    rotate(f.begin(), f.begin() + rnd(0, m - 1), f.end());
    int idx = cyc_tsearch(m, [&](int a, int b) { return f[a] < f[b]; });
    assert(0 <= idx && idx < m);
    assert(f[idx] == *min_element(f.begin(), f.end()));
    cases++; flatcases += flat;
  }
  printf("cyc_tsearch ok: %d cases (%d with a 2-wide minimum plateau), n<=200\n", cases, flatcases);
}
