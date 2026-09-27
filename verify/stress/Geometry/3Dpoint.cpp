#include "prelude.h"
#include "geo_prelude.h"
#include "stress.h"
#include "8_Geometry/3Dpoint.cpp"
// Conventions checked here: area() = 2 * triangle area, volume() = 6 * signed tetra
// volume (> 0 iff d is on the side of cross3(a,b,c)), phi/theta = spherical angles.
Point rp(int L) { return Point(rnd(-L, L), rnd(-L, L), rnd(-L, L)); }
Point rpd(double L) { return Point(rndd(-L, L), rndd(-L, L), rndd(-L, L)); }
bool near(Point a, Point b, double e = 1e-7) { return abs(a - b) < e; }
double det3(Point a, Point b, Point c) { // brute determinant by cofactors
  return a.x * (b.y * c.z - b.z * c.y) - a.y * (b.x * c.z - b.z * c.x) + a.z * (b.x * c.y - b.y * c.x);
}
int main() {
  const double PI = acos(-1);
  int cnt = 0;
  FOR (it, 1, 200000) {
    bool integ = it & 1;
    Point a = integ ? rp(5) : rpd(5), b = integ ? rp(5) : rpd(5), c = integ ? rp(5) : rpd(5), d = integ ? rp(5) : rpd(5);
    cnt++;
    // cross / dot
    Point cr = cross(a, b);
    assert(fabs(dot(cr, a)) < 1e-9 && fabs(dot(cr, b)) < 1e-9);
    assert(fabs(dot(cr, c) - det3(a, b, c)) < 1e-9);
    // area: |cross| = 2 * Heron area
    double la = abs(b - c), lb = abs(a - c), lc = abs(a - b), s = (la + lb + lc) / 2;
    double heron = sqrt(max(0.0, s * (s - la) * (s - lb) * (s - lc)));
    assert(fabs(area(a, b, c) - 2 * heron) < 1e-5);
    // volume: det of edge vectors = 6 * signed volume
    assert(fabs(volume(a, b, c, d) - det3(b - a, c - a, d - a)) < 1e-9);
    // masscenter
    assert(near(masscenter(a, b, c, d), Point((a.x + b.x + c.x + d.x) / 4, (a.y + b.y + c.y + d.y) / 4, (a.z + b.z + c.z + d.z) / 4)));
    // phi / theta reconstruct the direction
    if (abs(a) > 1e-9) {
      double ph = phi(a), th = theta(a), r = abs(a);
      assert(-PI <= ph && ph <= PI && 0 <= th && th <= PI);
      assert(near(Point(r * sin(th) * cos(ph), r * sin(th) * sin(ph), r * cos(th)), a));
    }
    // Point(pdd) lifts to the paraboloid
    { pdd q(a.x, a.y); Point L(q); assert(L.x == a.x && L.y == a.y && fabs(L.z - (a.x * a.x + a.y * a.y)) < 1e-12); }
    // proj: isometric 2D chart of plane abc with a -> (0,0), b on +x axis, c on +y side
    if (abs(cross3(a, b, c)) > 1e-6) {
      Point u = a + (b - a) * rndd(-2, 2) + (c - a) * rndd(-2, 2), v = a + (b - a) * rndd(-2, 2) + (c - a) * rndd(-2, 2);
      pdd pu = proj(a, b, c, u), pv = proj(a, b, c, v), pa = proj(a, b, c, a), pb = proj(a, b, c, b), pc = proj(a, b, c, c);
      assert(abs(pa) < 1e-9 && fabs(pb.Y) < 1e-9 && pb.X > 0 && pc.Y > 0);
      assert(fabs(abs(pu - pv) - abs(u - v)) < 1e-7);
      // point off the plane projects orthogonally
      Point n = cross3(a, b, c), w = u + n * rndd(-1, 1);
      pdd pw = proj(a, b, c, w);
      assert(abs(pw - pu) < 1e-7);
    }
    // rotate_around: Rodrigues, right-hand rule about axis
    if (abs(b) > 1e-6) {
      double ang = rndd(-7, 7);
      Point r = rotate_around(a, ang, b), u = b / abs(b);
      assert(fabs(abs(r) - abs(a)) < 1e-9);
      assert(fabs(dot(r, u) - dot(a, u)) < 1e-9);
      // brute: rotation matrix
      double C = cos(ang), S = sin(ang), t = 1 - C, x = u.x, y = u.y, z = u.z;
      Point m(
        (t * x * x + C) * a.x + (t * x * y - S * z) * a.y + (t * x * z + S * y) * a.z,
        (t * x * y + S * z) * a.x + (t * y * y + C) * a.y + (t * y * z - S * x) * a.z,
        (t * x * z - S * y) * a.x + (t * y * z + S * x) * a.y + (t * z * z + C) * a.z);
      assert(near(r, m));
    }
    { // rotate x-axis by 90 deg about z gives y-axis
      Point r = rotate_around(Point(1, 0, 0), PI / 2, Point(0, 0, 3));
      assert(near(r, Point(0, 1, 0)));
    }
  }
  printf("3Dpoint: %d cases OK (cross/dot/area/volume/masscenter/phi/theta/proj/rotate_around/lift)\n", cnt);
}
