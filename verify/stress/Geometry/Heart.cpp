// Heart.cpp: incenter / masscenter / orthcenter (circenter is LC-verified; checked here too
// since orthcenter is built on it). Non-degenerate triangles only (collinear => m = 0 in circenter).
#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/Heart.cpp"

double lineDist(pdd a, pdd b, pdd p) { return fabs(cross(b - a, p - a)) / abs(b - a); }
int main() {
  int cases = 0;
  FOR (it, 1, 300000) {
    pdd p[3];
    bool ints = it % 2;
    FOR (i, 0, 2) p[i] = ints ? pdd(rnd(-6, 6), rnd(-6, 6)) : pdd(rndd(-100, 100), rndd(-100, 100));
    double ar = cross(p[1] - p[0], p[2] - p[0]);
    if (fabs(ar) < (ints ? 0.5 : 1)) continue; // skip degenerate
    double sc = max({abs(p[1] - p[0]), abs(p[2] - p[0]), abs(p[2] - p[1])}), tol = 1e-7 * sc;
    // circenter: equidistant
    pdd O = circenter(p[0], p[1], p[2]);
    assert(fabs(abs(O - p[0]) - abs(O - p[1])) < tol && fabs(abs(O - p[0]) - abs(O - p[2])) < tol);
    // incenter: strictly inside, equidistant to the three side lines, r = area / s * 2 (s = perimeter)
    pdd In = incenter(p[0], p[1], p[2]);
    FOR (i, 0, 2) assert(ori(p[i], p[(i + 1) % 3], In) == (ar > 0 ? 1 : -1));
    double r = fabs(ar) / 2 / (abs(p[0] - p[1]) + abs(p[1] - p[2]) + abs(p[2] - p[0])) * 2;
    FOR (i, 0, 2) assert(fabs(lineDist(p[i], p[(i + 1) % 3], In) - r) < tol);
    // masscenter
    pdd G = masscenter(p[0], p[1], p[2]);
    assert(abs(G - (p[0] + p[1] + p[2]) / 3) < tol);
    // orthcenter: (H - p_i) . (p_j - p_k) = 0 for all i
    pdd H = orthcenter(p[0], p[1], p[2]);
    FOR (i, 0, 2) {
      pdd u = H - p[i], v = p[(i + 1) % 3] - p[(i + 2) % 3];
      // relative to |v| * scale of H (H can be far away for near-degenerate triangles)
      assert(fabs(dot(u, v)) <= 1e-7 * abs(v) * max(sc, abs(u)));
    }
    cases++;
  }
  printf("Heart: %d triangles OK\n", cases);
}
