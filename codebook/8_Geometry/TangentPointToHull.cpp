// C: hull(); p outside or on the hull
pii get_tangent(vector<pll> &C, pll p) {
  int n = SZ(C);
  pll d = p * 3 - C[0] - C[1 % n] - C[2 % n];
  auto gao = [&](int s) {
    return cyc_tsearch(n, [&](int x, int y) {
      __int128 v = cross(C[x] - p, C[y] - p);
      pll e = C[y] - C[x];
      if (!v) v = (__int128)e.X * d.Y -
        (__int128)e.Y * d.X;
      return s > 0 ? v > 0 : v < 0;
    });
  };
  return pii(gao(1), gao(-1));
} // return (a, b), ori(p, C[a], C[b]) >= 0
