double rat(pll a, pll b) {
  return sign(b.X) ? (double)a.X / b.X : (double)a.Y / b.Y;
} // all poly. should be ccw
double polyUnion(vector<vector<pll>> &poly) {
  double res = 0;
  for (auto &p : poly)
    FOR (a, 0, SZ(p) - 1) {
      pll A = p[a], B = p[(a + 1) % SZ(p)];
      vector<pair<double, int>> segs = {{0, 0}, {1, 0}};
      for (auto &q : poly) {
        if (&p == &q) continue;
        FOR (b, 0, SZ(q) - 1) {
          pll C = q[b], D = q[(b + 1) % SZ(q)];
          int sc = ori(A, B, C), sd = ori(A, B, D);
          if (sc != sd && min(sc, sd) < 0) {
            double sa = cross(D - C, A - C), sb = cross(D - C, B - C);
            segs.emplace_back(sa / (sa - sb), sign(sc - sd));
          }
          if (!sc && !sd && &q < &p && sign(dot(B - A, D - C)) > 0) {
            segs.emplace_back(rat(C - A, B - A), 1);
            segs.emplace_back(rat(D - A, B - A), -1);
          }
        }
      }
      sort(ALL(segs));
      for (auto &s : segs) s.X = clamp(s.X, 0.0, 1.0);
      double sum = 0;
      int cnt = segs[0].Y;
      FOR (j, 1, SZ(segs) - 1) {
        if (!cnt) sum += segs[j].X - segs[j - 1].X;
        cnt += segs[j].Y;
      }
      res += cross(A, B) * sum;
    }
  return res / 2;
}
