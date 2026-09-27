#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/Polar_Angle_Sort.cpp" // cmp()
#include "8_Geometry/HPIGeneralLine.cpp"
// The template only supplies LN / intersect / cov / operator<; the algorithm is the deque of
// Half_plane_intersection.cpp with Line -> LN, isin(p, a, b) -> !cov(p, a, b), sort(ALL(arr)).
// That is what a contestant pastes, so that is what is tested:
vector<LN> halfPlaneInter(vector<LN> arr) {
  sort(ALL(arr));
  deque<LN> dq(1, arr[0]);
  auto pop_back = [&](int t, LN p) {
    while (SZ(dq) >= t && cov(p, dq[SZ(dq) - 2], dq.back())) dq.pop_back();
  };
  auto pop_front = [&](int t, LN p) {
    while (SZ(dq) >= t && cov(p, dq[0], dq[1])) dq.pop_front();
  };
  for (auto p : arr)
    if (cmp(dq.back().dir(), p.dir(), 0) != -1)
      pop_back(2, p), pop_front(2, p), dq.pb(p);
  pop_back(3, dq[0]), pop_front(3, dq.back());
  return vector<LN>(ALL(dq));
}
// brute in coefficient form: a x + b y + c <= 0
double val(LN l, pdd p) { return l.a * p.X + l.b * p.Y + l.c; }
bool inside(LN l, pdd p, double e) { return val(l, p) <= e * hypot(l.a, l.b); }
double polyArea(vector<pdd> v) {
  double a = 0;
  FOR (i, 0, SZ(v) - 1) a += cross(v[i], v[(i + 1) % SZ(v)]);
  return a / 2;
}
double hullArea(vector<pdd> p) {
  sort(ALL(p));
  if (SZ(p) < 3) return 0;
  vector<pdd> h(2 * SZ(p)); int k = 0;
  FOR (s, 0, 1) {
    int st = k;
    for (auto q : p) { while (k >= st + 2 && cross(h[k - 1] - h[k - 2], q - h[k - 2]) <= 1e-12) k--; h[k++] = q; }
    k--; reverse(ALL(p));
  }
  h.resize(max(k, 0));
  return polyArea(h);
}
double brute(const vector<LN> &ls) { // -1: empty
  vector<pdd> pts;
  FOR (i, 0, SZ(ls) - 1) FOR (j, i + 1, SZ(ls) - 1) {
    double D = (double)ls[i].a * ls[j].b - (double)ls[j].a * ls[i].b;
    if (D == 0) continue;
    pdd p((-(double)ls[i].c * ls[j].b + (double)ls[j].c * ls[i].b) / D, (-(double)ls[i].a * ls[j].c + (double)ls[j].a * ls[i].c) / D);
    if (all_of(ALL(ls), [&](LN l) { return inside(l, p, 1e-9); })) pts.pb(p);
  }
  if (pts.empty()) return -1;
  return hullArea(pts);
}
int main() {
  int cnt = 0, stat[3] = {}, degenNonEmpty = 0;
  FOR (it, 1, 25000) {
    int mode = it % 3, L = mode == 0 ? 3 : mode == 1 ? 10 : 1000, B = mode == 2 ? 2000 : 2 * L;
    int n = rnd(1, mode == 2 ? 12 : 8);
    vector<LN> ls;
    FOR (i, 1, n) {
      if (rnd(0, 1)) { // through two lattice points
        pll p(rnd(-L, L), rnd(-L, L)), q(rnd(-L, L), rnd(-L, L));
        if (p == q) { i--; continue; }
        ls.pb(p, q);
      } else {         // general a x + b y + c <= 0, possibly non-primitive coefficients
        ll a = rnd(-5, 5), b = rnd(-5, 5), c = rnd(-5 * L, 5 * L), k = rnd(1, 3);
        if (!a && !b) { i--; continue; }
        ls.pb(a * k, b * k, c * k);
      }
      LN l = ls.back();
      if (rnd(0, 5) == 0) ls.pb(-l.a, -l.b, -l.c);                     // opposite half-plane
      if (rnd(0, 5) == 0) ls.pb(2 * l.a, 2 * l.b, 2 * l.c);            // same half-plane, scaled
      if (rnd(0, 5) == 0) ls.pb(l.a, l.b, l.c + rnd(-3, 3));           // parallel
    }
    ls.pb(0, -1, -B), ls.pb(1, 0, -B), ls.pb(0, 1, -B), ls.pb(-1, 0, -B); // |x|,|y| <= B
    shuffle(ALL(ls), rng);
    auto res = halfPlaneInter(ls);
    double want = brute(ls);
    cnt++;
    if (want > 1e-7) {
      stat[2]++;
      if (SZ(res) < 3) { printf("missing region, want %.6f\n", want); fflush(stdout); assert(0); }
      vector<pdd> v;
      FOR (i, 0, SZ(res) - 1) v.pb(intersect(res[i], res[(i + 1) % SZ(res)]));
      for (auto p : v) for (auto l : ls) assert(inside(l, p, 1e-6));
      double got = polyArea(v);
      if (fabs(got - want) > 1e-6 * max(1.0, want)) { printf("area got %.9f want %.9f\n", got, want); fflush(stdout); assert(0); }
    } else {
      stat[want < 0 ? 0 : 1]++;
      if (want < 0) assert(SZ(res) <= 2);
      else if (SZ(res) > 2) degenNonEmpty++;
    }
  }
  printf("HPIGeneralLine: %d cases OK (empty %d, zero-area %d [size>2 returned %d], positive %d)\n",
         cnt, stat[0], stat[1], degenNonEmpty, stat[2]);
}
