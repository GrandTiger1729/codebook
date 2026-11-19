#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/Polar_Angle_Sort.cpp" // cmp() (commented out of content.tex)
#include "8_Geometry/Half_plane_intersection.cpp"
// The same template on the integer prelude (Default_code_int, not in the book): Line holds
// pll, so area_pair / cmp / ori are exact ll. With the book's prelude (Line = pdd) they are
// doubles, exact only while |cross| < 2^53, i.e. |coord| <= ~3.3e7.
namespace ints {
#include "8_Geometry/Default_code_int.cpp"
#include "8_Geometry/Polar_Angle_Sort.cpp"
#include "8_Geometry/Half_plane_intersection.cpp"
}
// halfPlaneInter(lines): half-plane = left of Line.X -> Line.Y; integer coordinates.
// Result size <= 2 means "no (positive-area) region".
// Exact reference (__int128): all pairwise intersections lying in every half-plane. The
// region is empty iff there are none; it has zero area iff one input line contains all of
// them (a segment / point region lies on an input line). For a positive region the result
// must be a CCW cycle (consecutive directions turn left, one full turn) whose vertices all
// lie in every half-plane: then it is exactly the region (its half-planes are input ones).
// Magnitudes for |coord| <= M: D <= 8M^2, numerators <= 32M^3, side test <= 128M^4 < 2^127.
typedef __int128 L;
typedef pair<pll, pll> IL; // integer line p -> q
L cr(pll a, pll b) { return (L)a.X * b.Y - (L)a.Y * b.X; }
int sg(L x) { return (x > 0) - (x < 0); }
struct RP { L x, y, d; }; // rational point (x / d, y / d), d > 0
pll dir(IL l) { return l.Y - l.X; }
bool meet(IL A, IL B, RP &r) {
  pll d1 = dir(A), d2 = dir(B);
  L D = cr(d1, d2), t = cr(d1, A.X - B.X);
  if (!D) return false;
  r = {B.X.X * D + d2.X * t, B.X.Y * D + d2.Y * t, D};
  if (D < 0) r.x = -r.x, r.y = -r.y, r.d = -r.d;
  return true;
}
int side(IL l, RP P) { // > 0 strictly left of l, 0 on l
  pll d = dir(l);
  L wx = P.x - l.X.X * P.d, wy = P.y - l.X.Y * P.d;
  return sg(d.X * wy - d.Y * wx);
}
int half(pll v) { return v.Y < 0 || (v.Y == 0 && v.X < 0); }
bool angLess(pll a, pll b) { return half(a) != half(b) ? half(a) < half(b) : cr(a, b) > 0; }
// 0 empty, 1 zero area, 2 positive area
int brute(const vector<IL> &ls) {
  vector<RP> F;
  FOR (i, 0, SZ(ls) - 1) FOR (j, i + 1, SZ(ls) - 1) {
    RP P;
    if (meet(ls[i], ls[j], P) && all_of(ALL(ls), [&](IL l) { return side(l, P) >= 0; })) F.pb(P);
  }
  if (F.empty()) return 0;
  for (auto l : ls)
    if (all_of(ALL(F), [&](RP P) { return side(l, P) == 0; })) return 1;
  return 2;
}
// res is exactly the positive region of ls (see top)
void checkPolygon(const vector<IL> &ls, const vector<IL> &res) {
  int h = SZ(res), wraps = 0;
  assert(h >= 3);
  FOR (t, 0, h - 1) {
    IL a = res[t], b = res[(t + 1) % h];
    assert(cr(dir(a), dir(b)) > 0);
    wraps += angLess(dir(b), dir(a));
    RP V;
    assert(meet(a, b, V));
    for (auto l : ls) assert(side(l, V) >= 0);
  }
  assert(wraps == 1);
}
typedef vector<IL> (*Solver)(const vector<IL> &);
vector<IL> bookHPI(const vector<IL> &v) {
  vector<Line> a;
  for (auto [p, q] : v) a.pb(pdd(p.X, p.Y), pdd(q.X, q.Y));
  vector<IL> r;
  for (auto [p, q] : halfPlaneInter(a)) r.pb(pll(llround(p.X), llround(p.Y)), pll(llround(q.X), llround(q.Y)));
  return r;
}
vector<IL> intHPI(const vector<IL> &v) { return ints::halfPlaneInter(v); }

// n random lines through lattice points of [-L, L]^2, often through (or next to) an earlier
// line's point (+ opposite / same line / parallel copies), inside the box |x|, |y| <= B (B <= M keeps every input coordinate <= M).
vector<IL> gen(ll L, ll B, ll M, int n) {
  vector<IL> ls;
  auto ok = [&](pll p) { return llabs(p.X) <= M && llabs(p.Y) <= M; };
  FOR (i, 1, n) {
    pll p(rnd(-L, L), rnd(-L, L)), q(rnd(-L, L), rnd(-L, L));
    if (SZ(ls) && rnd(0, 2) == 0) { // through (or next to) an earlier point: near-concurrent
      IL o = ls[rnd(0, SZ(ls) - 1)];
      p = (rnd(0, 1) ? o.X : o.Y) + pll(rnd(-1, 1), rnd(-1, 1)) * rnd(0, 1);
      if (!ok(p)) continue;
    }
    if (p == q) { i--; continue; }
    ls.pb(p, q);
    pll d = q - p, e = d / __gcd(llabs(d.X), llabs(d.Y));
    if (rnd(0, 5) == 0) ls.pb(q, p);                               // opposite half-plane
    if (rnd(0, 5) == 0) {                                          // same line, other points
      ll k = rnd(1, 2 * __gcd(llabs(d.X), llabs(d.Y)));
      if (ok(p + e * k) && ok(q + e * k)) ls.pb(p + e * k, q + e * k);
    }
    if (rnd(0, 5) == 0) {                                          // parallel
      pll s(rnd(-2, 2), rnd(-2, 2));
      if (ok(p + s) && ok(q + s)) ls.pb(p + s, q + s);
    }
  }
  for (auto [a, b] : vector<IL>{{{-B, -B}, {B, -B}}, {{B, -B}, {B, B}}, {{B, B}, {-B, B}}, {{-B, B}, {-B, -B}}})
    ls.pb(a, b);
  shuffle(ALL(ls), rng);
  return ls;
}
int stat[3], degenNonEmpty;
void check(Solver hpi, const vector<IL> &ls) {
  auto res = hpi(ls);
  int want = brute(ls);
  stat[want]++;
  if (want == 2) checkPolygon(ls, res);
  else if (want == 0) assert(SZ(res) <= 2);
  else degenNonEmpty += SZ(res) > 2; // point / segment: not specified
}
// n lines tangent to circles around an integer point c (radius in [R, 3R]): c is strictly
// inside every half-plane, so the region is positive; most lines are redundant.
vector<IL> genBig(int n, ll M) {
  ll R = M / 4, cx = rnd(-M / 4, M / 4), cy = rnd(-M / 4, M / 4);
  vector<IL> ls;
  while (SZ(ls) < n) {
    double a = rndd(0, 2 * M_PI), r = R * (rnd(0, 999) ? rndd(1.2, 3) : rndd(1, 1.0001));
    double px = cx + r * cos(a), py = cy + r * sin(a), s = rndd(1, R);
    pll p(llround(px + s * sin(a)), llround(py - s * cos(a))), q(llround(px - s * sin(a)), llround(py + s * cos(a)));
    if (max({llabs(p.X), llabs(p.Y), llabs(q.X), llabs(q.Y)}) > M || p == q) continue;
    if (sg(cr(q - p, pll(cx, cy) - p)) <= 0) continue; // rounding may break the tangent
    ls.pb(p, q);
  }
  return ls;
}
int main() {
  int cnt = 0;
  struct { const char *name; Solver f; ll M; } cfg[2] = {{"book prelude", bookHPI, 33000000}, {"integer prelude", intHPI, 1000000000}};
  for (auto [name, f, M] : cfg) {
    // two lines through 0 with cross(d1, d2) = -4 but |d| ~ 2M: in doubles at M = 1e9 the
    // products round to equal values, cmp() calls them parallel and drops one (book prelude)
    FOR (sw, 0, 1) {
      vector<IL> ls = {{{-M, -M + 1}, {M, M - 1}}, {{-M + 1, -M + 2}, {M - 1, M - 2}}};
      if (sw) for (auto &l : ls) swap(l.X, l.Y);
      for (auto [a, b] : vector<IL>{{{-M, -M}, {M, -M}}, {{M, -M}, {M, M}}, {{M, M}, {-M, M}}, {{-M, M}, {-M, -M}}})
        ls.pb(a, b);
      check(f, ls), cnt++;
    }
    FOR (it, 1, 25000) { // small: L in {3, 10, 1000}, n <= 12 lines (+ copies)
      int mode = it % 3, L = mode == 0 ? 3 : mode == 1 ? 10 : 1000, B = mode == 2 ? 2000 : 2 * L;
      check(f, gen(L, B, M, rnd(1, mode == 2 ? 12 : 8))), cnt++;
    }
    FOR (it, 1, 3000) { // contest coordinates: |coord| <= M, up to 60 lines (+ copies)
      ll L = it % 2 ? M : rnd(1, M);
      check(f, gen(L, it % 3 ? M : rnd(L, M), M, rnd(1, it % 10 ? 12 : 60))), cnt++;
    }
    int bigLines = 0;
    FOR (it, 1, 6) { // n = 1e5 lines, region known to be positive (no O(n^3) brute)
      auto ls = genBig(it % 2 ? 100000 : 2000, M);
      if (it % 3 == 0) { // + two parallel opposite half-planes with a gap: empty
        ll y = rnd(-M / 2, M / 2);
        ls.pb(pll(M, y), pll(-M, y)), ls.pb(pll(-M, y + 1), pll(M, y + 1));
        shuffle(ALL(ls), rng);
        assert(SZ(f(ls)) <= 2);
      } else {
        auto res = f(ls);
        checkPolygon(ls, res), bigLines = max(bigLines, SZ(res));
      }
      cnt++;
    }
    printf("%s, |coord| <= %lld: big n = 1e5 OK (largest result %d lines)\n", name, M, bigLines);
  }
  printf("halfPlaneInter: %d cases OK (empty %d, zero-area %d [size>2 returned %d], positive %d)\n",
         cnt, stat[0], stat[1], degenNonEmpty, stat[2]);
}
