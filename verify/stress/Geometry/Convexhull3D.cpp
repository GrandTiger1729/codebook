#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/3Dpoint.cpp"
#include "8_Geometry/Convexhull3D.cpp"
// convex_hull_3D(P): triangulated hull; faces index the *member* P (which gets reordered).
// Precondition: not all points coplanar. Brute: O(n^4) supporting-plane enumeration
// on integer points -> polygon faces -> surface area, volume, #polygon faces.
// Exactness: Point is double and sign() uses eps = 1e-8, so volume() must be exact: for
// |coord| <= M its partial sums reach 32 M^3 < 2^53 iff M <= ~6.5e4 (a true 0 volume then
// stays 0). Tested at |coord| <= 65000. The brute is in __int128 (|normal|^2 ~ M^4).
typedef __int128 L;
typedef array<ll, 3> V3;
typedef array<L, 3> W3;
W3 sub(V3 a, V3 b) { return {(L)a[0] - b[0], (L)a[1] - b[1], (L)a[2] - b[2]}; }
W3 crs(W3 a, W3 b) { return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]}; }
L dt(W3 a, W3 b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
W3 w3(V3 a) { return {a[0], a[1], a[2]}; }
L gcd128(L a, L b) { a = a < 0 ? -a : a, b = b < 0 ? -b : b; while (b) a %= b, swap(a, b); return a; }
L abs128(L a) { return a < 0 ? -a : a; }
struct Res { double area, vol; int faces; };
Res brute(const vector<V3> &p) {
  int n = SZ(p);
  set<pair<W3, L>> planes;
  double cx = 0, cy = 0, cz = 0;
  for (auto &q : p) cx += q[0], cy += q[1], cz += q[2];
  cx /= n, cy /= n, cz /= n;
  Res r{0, 0, 0};
  FOR (i, 0, n - 1) FOR (j, i + 1, n - 1) FOR (k, j + 1, n - 1) {
    W3 nm = crs(sub(p[j], p[i]), sub(p[k], p[i]));
    if (nm == W3{0, 0, 0}) continue;
    int pos = 0, neg = 0;
    for (auto &q : p) { L s = dt(nm, sub(q, p[i])); pos += s > 0, neg += s < 0; }
    if (pos && neg) continue;
    if (pos) FOR (t, 0, 2) nm[t] = -nm[t]; // outward
    L g = gcd128(gcd128(nm[0], nm[1]), nm[2]);
    FOR (t, 0, 2) nm[t] /= g;
    L off = dt(nm, w3(p[i]));
    if (!planes.insert({nm, off}).second) continue;
    // polygon on this plane
    int drop = 0;
    FOR (t, 1, 2) if (abs128(nm[t]) > abs128(nm[drop])) drop = t;
    vector<pll> q2;
    for (auto &q : p) if (dt(nm, w3(q)) == off) {
      vector<ll> c; FOR (t, 0, 2) if (t != drop) c.pb(q[t]);
      q2.pb(c[0], c[1]);
    }
    sort(ALL(q2)), q2.erase(unique(ALL(q2)), q2.end());
    auto cr2 = [](pll o, pll a, pll b) { return (L)(a.X - o.X) * (b.Y - o.Y) - (L)(a.Y - o.Y) * (b.X - o.X); };
    vector<pll> h(2 * SZ(q2) + 1); int m = 0;
    FOR (s, 0, 1) {
      int st = m;
      for (auto &q : q2) { while (m >= st + 2 && cr2(h[m - 2], h[m - 1], q) <= 0) m--; h[m++] = q; }
      m--; reverse(ALL(q2));
    }
    L a2 = 0;
    FOR (s, 0, m - 1) a2 += (L)h[s].X * h[(s + 1) % m].Y - (L)h[s].Y * h[(s + 1) % m].X;
    long double len = sqrtl((long double)dt(nm, nm));
    double area = fabsl((long double)a2) / 2 * len / (long double)abs128(nm[drop]);
    double dist = fabsl((long double)off - ((long double)nm[0] * cx + (long double)nm[1] * cy + (long double)nm[2] * cz)) / len;
    r.area += area, r.vol += area * dist / 3, r.faces++;
  }
  return r;
}
V3 toV(Point p) { return {llround(p.x), llround(p.y), llround(p.z)}; }
bool coplanar(const vector<V3> &p) { // O(n): p[0], p[a], p[b] span a plane (if any)
  int a = 0, b = 0;
  FOR (i, 1, SZ(p) - 1) if (p[i] != p[0]) { a = i; break; }
  FOR (i, 1, SZ(p) - 1) if (a && crs(sub(p[a], p[0]), sub(p[i], p[0])) != W3{0, 0, 0}) { b = i; break; }
  if (!b) return true; // collinear
  FOR (k, 1, SZ(p) - 1) if (dt(crs(sub(p[a], p[0]), sub(p[b], p[0])), sub(p[k], p[0]))) return false;
  return true;
}
// Exact structural check (no brute): every face non-degenerate with all points on its inner
// side, every directed edge used once with its reverse used once (closed, consistently
// oriented). Then the surface is the hull (a convex polytope with input vertices containing
// every input point). Also polygon_face_num == #distinct face planes, get_volume == exact.
void checkStructure(convex_hull_3D &h) {
  vector<V3> P;
  for (auto q : h.P) P.pb(toV(q));
  map<pii, int> edges;
  set<pair<W3, L>> planes;
  L vol6 = 0;
  long double sa = 0;
  for (auto f : h.res) {
    W3 nm = crs(sub(P[f.b], P[f.a]), sub(P[f.c], P[f.a]));
    assert(nm != (W3{0, 0, 0}));
    for (auto &q : P) assert(dt(nm, sub(q, P[f.a])) <= 0);
    edges[{f.a, f.b}]++, edges[{f.b, f.c}]++, edges[{f.c, f.a}]++;
    L g = gcd128(gcd128(nm[0], nm[1]), nm[2]);
    FOR (t, 0, 2) nm[t] /= g;
    planes.insert({nm, dt(nm, w3(P[f.a]))});
    vol6 += dt(crs(w3(P[f.a]), w3(P[f.b])), w3(P[f.c]));
    sa += sqrtl((long double)dt(nm, nm)) * g / 2;
  }
  for (auto [e, c] : edges) assert(c == 1 && edges.count({e.S, e.F}) && edges[{e.S, e.F}] == 1);
  assert(h.polygon_face_num() == SZ(planes));
  assert(fabsl(h.get_volume() - fabsl((long double)vol6) / 6) < 1e-9 * fabsl((long double)vol6));
  long double sa2 = 0;
  for (auto f : h.res) sa2 += area(h.P[f.a], h.P[f.b], h.P[f.c]) / 2;
  assert(fabsl(sa - sa2) < 1e-9 * sa);
}
int main() {
  int cnt = 0, skipped = 0, big = 0, maxFaces = 0;
  const ll M = 65000;
  FOR (it, 1, 9000) { // it > 6000: |coord| <= M, many points on few planes
    int mode = it % 3, L = mode == 0 ? 2 : mode == 1 ? 4 : 100, n = rnd(4, mode == 2 ? 20 : 14);
    bool large = it > 6000;
    vector<V3> p(n);
    if (!large) {
      for (auto &q : p) q = {rnd(0, L), rnd(0, L), rnd(0, L)};
      if (mode == 0 && rnd(0, 1)) FOR (i, 0, n - 1) if (rnd(0, 2)) p[i][2] = 0; // many coplanar
    } else {
      L = M;
      for (auto &q : p) q = {rnd(-M, M), rnd(-M, M), rnd(-M, M)};
      if (mode) { // most points on 1-2 random planes through lattice points
        FOR (pl, 1, mode) {
          V3 o = {rnd(-M / 2, M / 2), rnd(-M / 2, M / 2), rnd(-M / 2, M / 2)}, u, v;
          FOR (t, 0, 2) u[t] = rnd(-30, 30), v[t] = rnd(-30, 30);
          ll lim = M / 2 / 31;
          FOR (i, 0, n - 1) if (rnd(0, 2)) {
            ll s1 = rnd(-lim, lim), s2 = rnd(-lim, lim);
            FOR (t, 0, 2) p[i][t] = o[t] + s1 * u[t] + s2 * v[t];
          }
        }
      }
    }
    if (coplanar(p)) { skipped++; continue; } // documented: all-coplanar input is WA/UB
    vector<Point> P;
    for (auto &q : p) P.pb(q[0], q[1], q[2]);
    convex_hull_3D h(P);
    Res want = brute(p);
    // outward orientation, closed 2-manifold (exact), polygon_face_num, volume
    checkStructure(h);
    double sa = 0;
    for (auto f : h.res) sa += area(h.P[f.a], h.P[f.b], h.P[f.c]) / 2;
    assert(fabs(sa - want.area) < 1e-6 * max(1.0, want.area));
    assert(fabs(h.get_volume() - want.vol) < 1e-6 * max(1.0, want.vol));
    assert(h.polygon_face_num() == want.faces);
    // get_dis: distance of a point to a face plane
    auto f = h.res[0]; Point q(rnd(-L, L), rnd(-L, L), rnd(-L, L));
    Point nn = cross3(h.P[f.a], h.P[f.b], h.P[f.c]);
    double d = fabs(dot(nn, q - h.P[f.a])) / abs(nn);
    assert(fabs(h.get_dis(q, f) - d) < 1e-7 * max(1.0, d));
    cnt++, big += large;
  }
  FOR (it, 1, 12) { // n up to 2000 (O(n^2) template), |coord| <= M: sphere / cube surface / ball
    int n = it % 2 ? 2000 : rnd(4, 2000);
    vector<V3> p(n);
    for (auto &q : p) {
      if (it % 3 == 0) { // near a sphere: many hull vertices
        double z = rndd(-1, 1), t = rndd(0, 2 * M_PI), r = sqrt(1 - z * z);
        q = {llround(M * r * cos(t)), llround(M * r * sin(t)), llround(M * z)};
      } else if (it % 3 == 1) { // lattice points on the surface of a cube: coplanar faces
        q = {rnd(-3, 3) * (M / 3), rnd(-3, 3) * (M / 3), rnd(-3, 3) * (M / 3)};
        q[rnd(0, 2)] = rnd(0, 1) ? M / 3 * 3 : -(M / 3 * 3);
      } else q = {rnd(-M, M), rnd(-M, M), rnd(-M, M)};
    }
    vector<Point> P;
    for (auto &q : p) P.pb(q[0], q[1], q[2]);
    convex_hull_3D h(P);
    checkStructure(h);
    maxFaces = max(maxFaces, SZ(h.res)), cnt++;
  }
  printf("Convexhull3D: %d hulls OK (%d with |coord|<=%lld vs O(n^4) brute; 12 with n<=2000 exact structure, up to %d faces); %d all-coplanar inputs skipped\n",
         cnt, big, M, maxFaces, skipped);
}
