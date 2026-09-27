#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/3Dpoint.cpp"
#include "8_Geometry/Convexhull3D.cpp"
// convex_hull_3D(P): triangulated hull; faces index the *member* P (which gets reordered).
// Precondition: not all points coplanar. Brute: O(n^4) supporting-plane enumeration
// on integer points -> polygon faces -> surface area, volume, #polygon faces.
typedef array<ll, 3> V3;
V3 sub(V3 a, V3 b) { return {a[0] - b[0], a[1] - b[1], a[2] - b[2]}; }
V3 crs(V3 a, V3 b) { return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]}; }
ll dt(V3 a, V3 b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
struct Res { double area, vol; int faces; };
Res brute(const vector<V3> &p) {
  int n = SZ(p);
  set<pair<V3, ll>> planes;
  double cx = 0, cy = 0, cz = 0;
  for (auto &q : p) cx += q[0], cy += q[1], cz += q[2];
  cx /= n, cy /= n, cz /= n;
  Res r{0, 0, 0};
  FOR (i, 0, n - 1) FOR (j, i + 1, n - 1) FOR (k, j + 1, n - 1) {
    V3 nm = crs(sub(p[j], p[i]), sub(p[k], p[i]));
    if (nm == V3{0, 0, 0}) continue;
    int pos = 0, neg = 0;
    for (auto &q : p) { ll s = dt(nm, sub(q, p[i])); pos += s > 0, neg += s < 0; }
    if (pos && neg) continue;
    if (pos) FOR (t, 0, 2) nm[t] = -nm[t]; // outward
    ll g = __gcd(__gcd(llabs(nm[0]), llabs(nm[1])), llabs(nm[2]));
    FOR (t, 0, 2) nm[t] /= g;
    ll off = dt(nm, p[i]);
    if (!planes.insert({nm, off}).second) continue;
    // polygon on this plane
    int drop = 0;
    FOR (t, 1, 2) if (llabs(nm[t]) > llabs(nm[drop])) drop = t;
    vector<pll> q2;
    for (auto &q : p) if (dt(nm, q) == off) {
      vector<ll> c; FOR (t, 0, 2) if (t != drop) c.pb(q[t]);
      q2.pb(c[0], c[1]);
    }
    sort(ALL(q2)), q2.erase(unique(ALL(q2)), q2.end());
    auto cr2 = [](pll o, pll a, pll b) { return (a.X - o.X) * (b.Y - o.Y) - (a.Y - o.Y) * (b.X - o.X); };
    vector<pll> h(2 * SZ(q2) + 1); int m = 0;
    FOR (s, 0, 1) {
      int st = m;
      for (auto &q : q2) { while (m >= st + 2 && cr2(h[m - 2], h[m - 1], q) <= 0) m--; h[m++] = q; }
      m--; reverse(ALL(q2));
    }
    ll a2 = 0;
    FOR (s, 0, m - 1) a2 += h[s].X * h[(s + 1) % m].Y - h[s].Y * h[(s + 1) % m].X;
    double len = sqrt((double)dt(nm, nm));
    double area = fabs((double)a2) / 2 * len / llabs(nm[drop]);
    double dist = fabs(off - (nm[0] * cx + nm[1] * cy + nm[2] * cz)) / len;
    r.area += area, r.vol += area * dist / 3, r.faces++;
  }
  return r;
}
int main() {
  int cnt = 0, skipped = 0;
  FOR (it, 1, 6000) {
    int mode = it % 3, L = mode == 0 ? 2 : mode == 1 ? 4 : 100, n = rnd(4, mode == 2 ? 20 : 14);
    vector<V3> p(n);
    for (auto &q : p) q = {rnd(0, L), rnd(0, L), rnd(0, L)};
    if (mode == 0 && rnd(0, 1)) FOR (i, 0, n - 1) if (rnd(0, 2)) p[i][2] = 0; // many coplanar
    bool flat = true;
    FOR (i, 1, n - 1) FOR (j, i + 1, n - 1) FOR (k, j + 1, n - 1)
      if (dt(crs(sub(p[i], p[0]), sub(p[j], p[0])), sub(p[k], p[0]))) flat = false;
    if (flat) { skipped++; continue; } // documented: all-coplanar input is WA/UB
    vector<Point> P;
    for (auto &q : p) P.pb(q[0], q[1], q[2]);
    convex_hull_3D h(P);
    Res want = brute(p);
    // outward orientation, closed 2-manifold
    map<pii, int> edges;
    double sa = 0;
    for (auto f : h.res) {
      for (auto &q : h.P) assert(sign(volume(h.P[f.a], h.P[f.b], h.P[f.c], q)) <= 0);
      edges[{f.a, f.b}]++, edges[{f.b, f.c}]++, edges[{f.c, f.a}]++;
      sa += area(h.P[f.a], h.P[f.b], h.P[f.c]) / 2;
    }
    for (auto [e, c] : edges) assert(c == 1 && edges.count({e.S, e.F}) && edges[{e.S, e.F}] == 1);
    assert(fabs(sa - want.area) < 1e-6 * max(1.0, want.area));
    assert(fabs(h.get_volume() - want.vol) < 1e-6 * max(1.0, want.vol));
    assert(h.polygon_face_num() == want.faces);
    // get_dis: distance of a point to a face plane
    auto f = h.res[0]; Point q(rnd(-L, L), rnd(-L, L), rnd(-L, L));
    Point nn = cross3(h.P[f.a], h.P[f.b], h.P[f.c]);
    assert(fabs(h.get_dis(q, f) - fabs(dot(nn, q - h.P[f.a])) / abs(nn)) < 1e-7);
    cnt++;
  }
  printf("Convexhull3D: %d hulls OK vs O(n^4) brute (area, volume, polygon_face_num, orientation, closedness); %d all-coplanar inputs skipped\n", cnt, skipped);
}
