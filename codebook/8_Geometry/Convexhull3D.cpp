struct convex_hull_3D {
struct Face {
  int a, b, c;
  Face(int ta, int tb, int tc) : a(ta), b(tb), c(tc) {}
}; // return the faces with pt indexes
vector<Face> res;
vector<Point> P;
convex_hull_3D(const vector<Point> &_P) : res(), P(_P) {
// all points coplanar case will WA, O(n^2)
  int n = SZ(P);
  if (n <= 2) return; // be careful about edge case
  // ensure first 4 points are not coplanar
  swap(P[1], *find_if(ALL(P), [&](auto p)
    { return sign(abs(P[0] - p)) != 0; }));
  swap(P[2], *find_if(ALL(P), [&](auto p)
    { return sign(abs(cross3(p, P[0], P[1]))) != 0; }));
  swap(P[3], *find_if(ALL(P), [&](auto p)
    { return sign(volume(P[0], P[1], P[2], p)) != 0; }));
  vector<vector<int>> flag(n, vector<int>(n));
  res.emplace_back(0, 1, 2); res.emplace_back(2, 1, 0);
  FOR (i, 3, n - 1) {
    vector<Face> next;
    for (auto [a, b, c] : res) {
      int d = sign(volume(P[a], P[b], P[c], P[i]));
      if (d <= 0) next.pb(a, b, c);
      flag[a][b] = flag[b][c] = flag[c][a] = d;
    }
    for (auto f : res) {
      auto add = [&](int x, int y) {
        if (flag[x][y] > 0 && flag[y][x] <= 0)
          next.emplace_back(x, y, i);
      };
      add(f.a, f.b);
      add(f.b, f.c);
      add(f.c, f.a);
    }
    res = next;
  }
}
bool same(Face s, Face t) {
  for (int x : {t.a, t.b, t.c})
    if (sign(volume(P[s.a], P[s.b], P[s.c], P[x])))
      return 0;
  return 1;
}
int polygon_face_num() {
  int ans = 0;
  FOR (i, 0, SZ(res) - 1)
    ans += none_of(res.begin(), res.begin() + i,
      [&](Face g) { return same(res[i], g); });
  return ans;
}
double get_volume() {
  double ans = 0;
  for (auto f : res)
    ans += volume(Point(0, 0, 0), P[f.a], P[f.b], P[f.c]);
  return fabs(ans / 6);
}
double get_dis(Point p, Face f) {
  Point a = P[f.a], b = P[f.b], c = P[f.c];
  return fabs(volume(a, b, c, p)) / area(a, b, c);
}
};
// n^2 delaunay: facets with negative z normal of
// convexhull of (x, y, x^2 + y^2), use a pseudo-point
// (0, 0, inf) to avoid degenerate case
