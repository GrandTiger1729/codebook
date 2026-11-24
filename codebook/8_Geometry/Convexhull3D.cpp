struct convex_hull_3D {
struct Face { int a, b, c; };
vector<Face> res; // faces, indexing the reordered P
vector<Point> P;
convex_hull_3D(const vector<Point> &_P) : P(_P) {
  // O(n^2); needs 4 non-coplanar points, else UB
  int n = SZ(P);
  if (n <= 3) return;
  swap(P[1], *find_if(ALL(P), [&](auto p)
    { return sign(abs(P[0] - p)) != 0; }));
  swap(P[2], *find_if(ALL(P), [&](auto p)
    { return sign(abs(cross3(p, P[0], P[1]))) != 0; }));
  swap(P[3], *find_if(ALL(P), [&](auto p)
    { return sign(volume(P[0], P[1], P[2], p)) != 0; }));
  vector<vector<int>> flag(n, vector<int>(n));
  res = {{0, 1, 2}, {2, 1, 0}};
  FOR (i, 3, n - 1) {
    vector<Face> next;
    for (auto [a, b, c] : res) {
      int d = sign(volume(P[a], P[b], P[c], P[i]));
      if (d <= 0) next.push_back({a, b, c});
      flag[a][b] = flag[b][c] = flag[c][a] = d;
    }
    for (auto [a, b, c] : res) {
      int v[] = {a, b, c, a};
      FOR (k, 0, 2) if (flag[v[k]][v[k + 1]] > 0 &&
                        flag[v[k + 1]][v[k]] <= 0)
        next.push_back({v[k], v[k + 1], i});
    }
    res = next;
  }
}
int polygon_face_num() {
  int ans = 0;
  FOR (i, 0, SZ(res) - 1) {
    Face f = res[i];
    ans += none_of(&res[0], &res[i], [&](Face g) {
      for (int x : {g.a, g.b, g.c})
        if (sign(volume(P[f.a], P[f.b], P[f.c], P[x])))
          return 0;
      return 1;
    });
  }
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
