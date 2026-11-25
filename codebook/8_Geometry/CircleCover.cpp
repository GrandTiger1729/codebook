// a[k]: area covered by >= k circles, O(n^2 log n)
// needs circleInter (Intersection of two circles)
vector<double> circleCover(vector<Cir> c) {
  int n = c.size();
  vector<double> a(n + 2);
  auto in = [&](int i, int j) { // c[j] in c[i], tie: index
    int s = sign(c[i].R - c[j].R);
    return (s > 0 || (s == 0 && i < j)) &&
      sign(c[i].R - c[j].R - abs(c[i].O - c[j].O)) >= 0;
  };
  FOR (i, 0, n - 1) {
    pdd o = c[i].O; double R = c[i].R, PI = acos(-1);
    int cnt = 1;
    vector<tuple<double, pdd, int>> e;
    FOR (j, 0, n - 1) if (j != i) {
      if (in(j, i)) cnt++;
      else if (!in(i, j) &&
               sign(abs(o - c[j].O) - R - c[j].R) < 0) {
        pdd p, q;
        circleInter(c[i], c[j], p, q);
        double A = atan2(p.Y - o.Y, p.X - o.X);
        double B = atan2(q.Y - o.Y, q.X - o.X);
        e.pb(B, q, 1), e.pb(A, p, -1), cnt += B > A;
      }
    }
    if (e.empty()) { a[cnt] += PI * R * R; continue; }
    sort(e.begin(), e.end()), e.pb(e[0]);
    FOR (j, 0, (int)e.size() - 2) {
      auto [s, p, d] = e[j];
      auto [t, q, _] = e[j + 1];
      double th = t - s + (t < s) * 2 * PI;
      cnt += d;
      a[cnt] += cross(p, q) / 2 + (th - sin(th)) * R * R / 2;
    }
  }
  return a;
}
