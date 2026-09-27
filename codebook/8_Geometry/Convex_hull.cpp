void hull(vector<pll> &p) { // n=1 => p = {}
  sort(p.begin(), p.end());
  vector<pll> h(1, p[0]);
  for (int ct = 0; ct < 2; ct++, reverse(ALL(p)))
    for (int i = 1, t = SZ(h); i < SZ(p); h.pb(p[i++]))
      while (SZ(h) > t &&
        ori(h[SZ(h) - 2], h.back(), p[i]) <= 0)
        h.pop_back();
  h.pop_back(), h.swap(p);
}
