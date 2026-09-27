#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
const double pi = acos(-1); // CircleCover uses `pi`, defined nowhere in the book
#include "8_Geometry/Intersection_of_two_circles.cpp"
#include "8_Geometry/CircleCover.cpp"
// Area[k] = area covered by >= k circles. Brute: integrate over x; between critical
// x's the covered length is smooth up to sqrt end-point singularities, which the
// substitution x = m - h cos t removes; then Gauss-Legendre on t.
CircleCover cc;
vector<double> gx, gw;
void gaussInit(int n) { // Gauss-Legendre nodes on [-1,1]
  gx.resize(n), gw.resize(n);
  FOR (i, 0, n - 1) {
    double z = cos(pi * (i + 0.75) / (n + 0.5)), pp = 0;
    FOR (it, 0, 100) {
      double p1 = 1, p2 = 0;
      FOR (j, 1, n) { double p3 = p2; p2 = p1; p1 = ((2 * j - 1) * z * p2 - (j - 1) * p3) / j; }
      pp = n * (z * p1 - p2) / (z * z - 1);
      double z1 = z; z = z1 - p1 / pp;
      if (fabs(z - z1) < 1e-15) break;
    }
    gx[i] = z, gw[i] = 2 / ((1 - z * z) * pp * pp);
  }
}
vector<double> brute(vector<Cir> cs) {
  int n = SZ(cs);
  vector<double> xs, res(n + 2, 0);
  for (auto &c : cs) xs.pb(c.O.X - c.R), xs.pb(c.O.X + c.R);
  FOR (i, 0, n - 1) FOR (j, i + 1, n - 1) {
    pdd a, b;
    if (cs[i].O != cs[j].O && CCinter(cs[i], cs[j], a, b)) xs.pb(a.X), xs.pb(b.X);
  }
  sort(ALL(xs));
  auto len = [&](double x, vector<double> &out) { // covered length by >=k at vertical line x
    vector<pair<double, int>> ev;
    for (auto &c : cs) {
      double h = c.R * c.R - (x - c.O.X) * (x - c.O.X);
      if (h <= 0) continue;
      h = sqrt(h);
      ev.pb(c.O.Y - h, 1), ev.pb(c.O.Y + h, -1);
    }
    sort(ALL(ev));
    int k = 0;
    FOR (i, 0, SZ(ev) - 1) {
      k += ev[i].S;
      if (i + 1 < SZ(ev)) FOR (j, 1, k) out[j] += ev[i + 1].F - ev[i].F;
    }
  };
  FOR (i, 0, SZ(xs) - 2) {
    double a = xs[i], b = xs[i + 1];
    if (b - a < 1e-12) continue;
    double m = (a + b) / 2, h = (b - a) / 2;
    FOR (q, 0, SZ(gx) - 1) {
      double t = (gx[q] + 1) * pi / 2; // t in [0, pi]
      double x = m - h * cos(t), w = gw[q] * pi / 2 * h * sin(t);
      vector<double> out(n + 2, 0);
      len(x, out);
      FOR (k, 1, n) res[k] += out[k] * w;
    }
  }
  return res;
}
int main() {
  gaussInit(24);
  int cnt = 0; double worst = 0;
  FOR (it, 1, 1500) {
    int mode = it % 3, n = rnd(1, mode == 2 ? 8 : 6);
    vector<Cir> cs(n);
    for (auto &c : cs) {
      if (mode < 2) c.O = pdd(rnd(-3, 3), rnd(-3, 3)), c.R = mode == 0 ? rnd(1, 4) : sqrt(rnd(1, 12));
      else c.O = pdd(rndd(-3, 3), rndd(-3, 3)), c.R = rndd(0.3, 4);
    }
    if (n > 1 && rnd(0, 3) == 0) cs[1] = cs[0]; // identical circles
    cc.init(n);
    FOR (i, 0, n - 1) cc.c[i] = cs[i];
    cc.solve();
    auto want = brute(cs);
    cnt++;
    FOR (k, 1, n) {
      double err = fabs(cc.Area[k] - want[k]);
      worst = max(worst, err);
      if (err > 1e-6 * max(1.0, want[k])) {
        printf("mismatch k=%d got=%.9f want=%.9f circles:", k, cc.Area[k], want[k]);
        for (auto &c : cs) printf(" (%g,%g,%g)", c.O.X, c.O.Y, c.R);
        puts(""); fflush(stdout); assert(0);
      }
    }
  }
  printf("CircleCover: %d cases OK vs x-sweep integration, max err %.2e\n", cnt, worst);
}
