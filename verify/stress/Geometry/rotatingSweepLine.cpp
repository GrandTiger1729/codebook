#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/Polar_Angle_Sort.cpp" // cmp()
// Instrumentation: the template's only observable step is
//   swap(pos[l.X], pos[l.Y]); swap(id[pos[l.X]], id[pos[l.Y]]);
// a non-template ::swap(int&, int&) wins overload resolution; each pair of calls
// is logged as {old pos[Y], old pos[X], Y, X} (the values the event writes).
vector<array<int, 4>> evlog;
int pend = -1, pa, pb;
void swap(int &a, int &b) {
  if (pend < 0) pa = a, pb = b, pend = 1;
  else evlog.push_back({pb, pa, a, b}), pend = -1;
  int t = a; a = b; b = t;
}
#include "8_Geometry/rotatingSweepLine.cpp"
// Contract: for each ordered pair (i, j) in polar order of ps[j] - ps[i] (starting at +x),
// i and j are adjacent in the order sorted by cross(dir, p) with pos[i] + 1 == pos[j],
// and get swapped. Requires distinct points, no three collinear.
typedef pair<ll, ll> P;
ll crs(P a, P b) { return a.X * b.Y - a.Y * b.X; }
bool sortedAfter(P u, const vector<int> &ord, const vector<P> &p, bool before0) {
  FOR (i, 0, SZ(ord) - 2) {
    P w(p[ord[i + 1]].X - p[ord[i]].X, p[ord[i + 1]].Y - p[ord[i]].Y);
    if (before0) { if (!(w.Y > 0 || (w.Y == 0 && w.X > 0))) return false; continue; }
    ll c = crs(u, w);
    if (c < 0 || (c == 0 && u.X * w.X + u.Y * w.Y >= 0)) return false;
  }
  return true;
}
// returns true iff every check passes
bool run(vector<pii> ps) {
  int n = SZ(ps);
  vector<P> p(ALL(ps));
  evlog.clear();
  rotatingSweepLine(ps);
  if (SZ(evlog) != n * (n - 1)) return false;
  vector<int> ord(n); iota(ALL(ord), 0);
  sort(ALL(ord), [&](int a, int b) { return p[a].Y != p[b].Y ? p[a].Y < p[b].Y : p[a].X < p[b].X; });
  if (!sortedAfter(P(1, 0), ord, p, true)) return false;
  set<pii> seen;
  FOR (e, 0, SZ(evlog) - 1) {
    auto [newX, newY, ly, lx] = evlog[e];
    int posX = newY, posY = newX; // old positions
    if (lx == ly || !seen.insert({lx, ly}).second) return false;
    if (posX + 1 != posY || ord[posX] != lx || ord[posY] != ly) return false;
    std::swap(ord[posX], ord[posY]);
    P u(p[ly].X - p[lx].X, p[ly].Y - p[lx].Y);
    if (e + 1 < SZ(evlog)) {
      auto [a, b, ny, nx] = evlog[e + 1];
      P v(p[ny].X - p[nx].X, p[ny].Y - p[nx].Y);
      int c = cmp(u, v, false);
      if (c == 0) return false;        // polar order must be non-decreasing
      if (c == -1) continue;           // same angle: finish the group first
    }
    if (!sortedAfter(u, ord, p, false)) return false;
  }
  return true;
}
int main() {
  int cnt = 0, degTot = 0, degBad = 0;
  vector<pii> firstBad;
  FOR (it, 1, 2500) {
    int L = it % 2 ? 6 : 1000, n = rnd(1, it % 2 ? 9 : 25);
    set<pii> s;
    FOR (i, 1, n) s.insert(pii(rnd(-L, L), rnd(-L, L)));
    vector<pii> ps(ALL(s));
    shuffle(ALL(ps), rng);
    n = SZ(ps);
    bool col = false;
    FOR (i, 0, n - 1) FOR (j, i + 1, n - 1) FOR (k, j + 1, n - 1)
      if (crs(P(ps[j].X - ps[i].X, ps[j].Y - ps[i].Y), P(ps[k].X - ps[i].X, ps[k].Y - ps[i].Y)) == 0) col = true;
    bool ok = run(ps);
    if (!col) { assert(ok); cnt++; }
    else { degTot++; if (!ok && !degBad++) firstBad = ps; }
  }
  { // smallest degenerate input: three collinear points
    vector<pii> t = {{0, 0}, {1, 0}, {2, 0}};
    printf("[note] 3 collinear points {(0,0),(1,0),(2,0)}: %s; ", run(t) ? "ok" : "order broken");
  }
  printf("rotatingSweepLine: %d general-position sets OK; with collinear triples %d/%d sets broken\n", cnt, degBad, degTot);
}
