// simple polygon P in any orientation: 1 if p is inside,
// 0 outside, -1 on the boundary; O(n)
int PointInPolygon(const vector<pll> &P, pll p) {
  int n = SZ(P), in = 0;
  FOR (i, 0, n - 1) {
    pll a = P[i], b = P[(i + 1) % n];
    if (btw(a, b, p)) return -1;
    if (a.Y > b.Y) swap(a, b); // ray from p towards +x
    if (a.Y <= p.Y && p.Y < b.Y && ori(a, b, p) > 0)
      in ^= 1;
  }
  return in;
}
