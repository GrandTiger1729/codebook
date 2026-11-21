#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/Polar_Angle_Sort.cpp" // cmp()
#include "8_Geometry/HPIGeneralLine.cpp"
// With the book's prelude cross(pll, pll) converts to pdd, so LN(p, q).c, cmp() and cov()'s
// cross(A.dir(), B.dir()) are doubles: exact only while |a|, |b| <= ~6.7e7 (2ab < 2^53),
// i.e. lattice |coord| <= ~3.3e7. Second config: the integer prelude (Default_code_int, not
// in the book), exact to lattice |coord| ~ 1e9. cov() compares its __int128 directly: it used
// to call sign(ll), which truncated sums above 2^63 (fixed 2026-11-22).
namespace ints {
#include "8_Geometry/Default_code_int.cpp"
#include "8_Geometry/Polar_Angle_Sort.cpp"
#include "8_Geometry/HPIGeneralLine.cpp"
int cmpDir(LN a, LN b) { return cmp(a.dir(), b.dir(), 0); }
}
int cmpDir(LN a, LN b) { return cmp(a.dir(), b.dir(), 0); }
// The template only supplies LN / intersect / cov / operator<; the algorithm is the deque of
// Half_plane_intersection.cpp with Line -> LN, isin(p, a, b) -> !cov(p, a, b), sort(ALL(arr)).
// That is what a contestant pastes, so that is what is tested (cmpDir = cmp on dir()):
template <class LN> vector<LN> halfPlaneInter(vector<LN> arr) {
  sort(ALL(arr));
  deque<LN> dq(1, arr[0]);
  auto pop_back = [&](int t, LN p) {
    while (SZ(dq) >= t && cov(p, dq[SZ(dq) - 2], dq.back())) dq.pop_back();
  };
  auto pop_front = [&](int t, LN p) {
    while (SZ(dq) >= t && cov(p, dq[0], dq[1])) dq.pop_front();
  };
  for (auto p : arr)
    if (cmpDir(dq.back(), p) != -1)
      pop_back(2, p), pop_front(2, p), dq.pb(p);
  pop_back(3, dq[0]), pop_front(3, dq.back());
  return vector<LN>(ALL(dq));
}
// Design range: cov() sums three __int128 terms of size 2|c|max(|a|,|b|)^2, so
// |a|, |b| <= 2e9 allows |c| up to ~7e18 (lines through lattice points of |coord| <= 1e9
// have |a|, |b| <= 2e9, |c| <= 2e18). Tested: |a|, |b| <= 2e9, |c| <= 4e18.
// Exact reference (__int128), a x + b y + c <= 0: all pairwise intersections lying in every
// half-plane. Empty iff there are none; zero area iff one input line contains all of them.
// A positive region must come back as a cycle of outward normals turning CCW (one full
// turn) whose vertices lie in every half-plane; then it is exactly the region.
// Magnitudes: D <= 2A^2, numerators <= 2AC, side test <= 4A^2 C + 2A^2 C < 2^127.
typedef __int128 L;
typedef array<ll, 3> GL; // a, b, c
struct RP { L x, y, d; };  // (x / d, y / d), d > 0
L cr(pll a, pll b) { return (L)a.X * b.Y - (L)a.Y * b.X; }
int sg(L x) { return (x > 0) - (x < 0); }
pll nrm(GL l) { return pll(l[0], l[1]); }
bool meet(GL A, GL B, RP &r) {
  L D = (L)A[0] * B[1] - (L)B[0] * A[1];
  if (!D) return false;
  r = {(L)A[1] * B[2] - (L)B[1] * A[2], (L)A[2] * B[0] - (L)B[2] * A[0], D};
  if (D < 0) r.x = -r.x, r.y = -r.y, r.d = -r.d;
  return true;
}
int side(GL l, RP P) { return sg(l[0] * P.x + l[1] * P.y + l[2] * P.d); } // <= 0 inside
int half(pll v) { return v.Y < 0 || (v.Y == 0 && v.X < 0); }
bool angLess(pll a, pll b) { return half(a) != half(b) ? half(a) < half(b) : cr(a, b) > 0; }
int brute(const vector<GL> &ls) { // 0 empty, 1 zero area, 2 positive
  vector<RP> F;
  FOR (i, 0, SZ(ls) - 1) FOR (j, i + 1, SZ(ls) - 1) {
    RP P;
    if (meet(ls[i], ls[j], P) && all_of(ALL(ls), [&](GL l) { return side(l, P) <= 0; })) F.pb(P);
  }
  if (F.empty()) return 0;
  for (auto l : ls)
    if (all_of(ALL(F), [&](RP P) { return side(l, P) == 0; })) return 1;
  return 2;
}
void checkPolygon(const vector<GL> &ls, const vector<GL> &res) {
  int h = SZ(res), wraps = 0;
  assert(h >= 3);
  FOR (t, 0, h - 1) {
    GL a = res[t], b = res[(t + 1) % h];
    assert(cr(nrm(a), nrm(b)) > 0);
    wraps += angLess(nrm(b), nrm(a));
    RP V;
    assert(meet(a, b, V));
    for (auto l : ls) assert(side(l, V) <= 0);
  }
  assert(wraps == 1);
}
template <class LN> vector<GL> solve(const vector<GL> &v) {
  vector<LN> a;
  for (auto l : v) a.pb(l[0], l[1], l[2]);
  vector<GL> r;
  for (auto l : halfPlaneInter(a)) r.pb(GL{l.a, l.b, l.c});
  return r;
}
typedef vector<GL> (*Solver)(const vector<GL> &);
struct Cfg { const char *name; Solver f; ll M, A; }; // lattice |coord| <= M, |a|, |b| <= A
int stat[3], degenNonEmpty;
void check(Solver f, const vector<GL> &ls) {
  auto res = f(ls);
  int want = brute(ls);
  stat[want]++;
  if (want == 2) checkPolygon(ls, res);
  else if (want == 0) assert(SZ(res) <= 2);
  else degenNonEmpty += SZ(res) > 2;
}
// small: lattice points in [-L, L], general a, b in [-5, 5]. contest (big = true): lattice
// points in [-M, M], general a, b in [-A, A] through a point of the box + offset.
vector<GL> gen(Cfg cf, ll L, ll B, int n, bool big) {
  vector<GL> ls;
  ll Amax = big ? 2 * cf.A : LLONG_MAX, Cmax = big ? 4 * cf.A * cf.M : LLONG_MAX;
  auto add = [&](ll a, ll b, ll c) {
    if (llabs(a) <= Amax && llabs(b) <= Amax && llabs(c) <= Cmax) ls.pb(GL{a, b, c});
  };
  FOR (i, 1, n) {
    if (rnd(0, 1)) { // through two lattice points
      pll p(rnd(-L, L), rnd(-L, L)), q(rnd(-L, L), rnd(-L, L));
      if (SZ(ls) && rnd(0, 2) == 0) p = pll(rnd(-1, 1), rnd(-1, 1)); // near-concurrent at 0
      if (p == q) { i--; continue; }
      ll a = (q - p).Y, b = -(q - p).X;
      add(a, b, -(a * p.X + b * p.Y));
    } else if (!big) { // general a x + b y + c <= 0, possibly non-primitive coefficients
      ll a = rnd(-5, 5), b = rnd(-5, 5), c = rnd(-5 * L, 5 * L), k = rnd(1, 3);
      if (!a && !b) { i--; continue; }
      add(a * k, b * k, c * k);
    } else {
      ll a = rnd(-cf.A, cf.A), b = rnd(-cf.A, cf.A), x = rnd(-L, L), y = rnd(-L, L);
      if (!a && !b) { i--; continue; }
      if (rnd(0, 2) == 0) x = y = 0;
      add(a, b, -(a * x + b * y) + rnd(-cf.A, cf.A) * rnd(0, 1));
    }
    if (SZ(ls) == 0) { i--; continue; }
    GL l = ls.back();
    if (rnd(0, 5) == 0) add(-l[0], -l[1], -l[2]);         // opposite half-plane
    if (rnd(0, 5) == 0) add(2 * l[0], 2 * l[1], 2 * l[2]); // same half-plane, scaled
    if (rnd(0, 5) == 0) add(l[0], l[1], l[2] + rnd(-3, 3)); // parallel
  }
  ls.pb(GL{0, -1, -B}), ls.pb(GL{1, 0, -B}), ls.pb(GL{0, 1, -B}), ls.pb(GL{-1, 0, -B}); // |x|,|y| <= B
  shuffle(ALL(ls), rng);
  return ls;
}
// n half-planes with random normals |a|, |b| <= A whose boundary passes at a random
// distance from an integer centre (strictly inside all): region positive, mostly redundant.
vector<GL> genBig(Cfg cf, int n) {
  ll cx = rnd(-cf.M / 4, cf.M / 4), cy = rnd(-cf.M / 4, cf.M / 4);
  vector<GL> ls;
  while (SZ(ls) < n) {
    ll a = rnd(-cf.A, cf.A), b = rnd(-cf.A, cf.A);
    double len = hypot(a, b), r = cf.M / 4 * (rnd(0, 999) ? rndd(1.2, 3) : rndd(1, 1.0001));
    if (!a && !b) continue;
    ls.pb(GL{a, b, -(a * cx + b * cy) - max(1LL, llround(r * len))});
  }
  return ls;
}
int main() {
  int cnt = 0;
  Cfg cfg[2] = {{"book prelude", solve<LN>, 33000000, 33000000}, {"integer prelude", solve<ints::LN>, 1000000000, 1000000000}};
  for (auto cf : cfg) {
    FOR (it, 1, 25000) {
      int mode = it % 3, L = mode == 0 ? 3 : mode == 1 ? 10 : 1000, B = mode == 2 ? 2000 : 2 * L;
      check(cf.f, gen(cf, L, B, rnd(1, mode == 2 ? 12 : 8), false)), cnt++;
    }
    FOR (it, 1, 3000) { // contest size
      ll L = it % 2 ? cf.M : rnd(1, cf.M);
      check(cf.f, gen(cf, L, it % 3 ? cf.M : rnd(L, cf.M), rnd(1, it % 10 ? 12 : 60), true)), cnt++;
    }
    int bigRes = 0;
    FOR (it, 1, 6) { // n = 1e5, region known positive / empty (no O(n^3) brute)
      auto ls = genBig(cf, it % 2 ? 100000 : 2000);
      if (it % 3 == 0) { // y <= k and y >= k + 1: empty
        ll k = rnd(-cf.M / 2, cf.M / 2);
        ls.pb(GL{0, 1, -k}), ls.pb(GL{0, -1, k + 1});
        shuffle(ALL(ls), rng);
        assert(SZ(cf.f(ls)) <= 2);
      } else {
        auto res = cf.f(ls);
        checkPolygon(ls, res), bigRes = max(bigRes, SZ(res));
      }
      cnt++;
    }
    printf("%s, |coord| <= %lld, |a|,|b| <= %lld: big n = 1e5 OK (largest result %d)\n", cf.name, cf.M, 2 * cf.A, bigRes);
  }
  printf("HPIGeneralLine: %d cases OK (empty %d, zero-area %d [size>2 returned %d], positive %d)\n",
         cnt, stat[0], stat[1], degenNonEmpty, stat[2]);
}
