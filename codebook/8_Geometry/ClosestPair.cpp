// closest pair; only needs pll + X/Y
pair<pll, pll> closest_pair(vector<pll> v) {
  sort(ALL(v), [](pll a, pll b) { return a.Y < b.Y; });
  pair<ll, pair<pll, pll>> r{LLONG_MAX, {}};
  set<pll> s;
  int j = 0;
  for (pll p : v) {
    ll d = 1 + (ll)sqrt(r.F);
    while (v[j].Y <= p.Y - d) s.erase(v[j++]);
    auto lo = s.lower_bound(pll(p.X - d, p.Y));
    auto hi = s.upper_bound(pll(p.X + d, p.Y));
    for (; lo != hi; lo++) {
      ll dx = lo->X - p.X, dy = lo->Y - p.Y;
      r = min(r, {dx * dx + dy * dy, {*lo, p}});
    }
    s.insert(p);
  }
  return r.S; // r.F is the squared distance
}
