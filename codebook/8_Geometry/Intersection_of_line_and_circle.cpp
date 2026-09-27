vector<pdd> circleLine(pdd c, double r, pdd a, pdd b) {
  pdd d = b - a, p = projection(a, b, c);
  double s = cross(d, c - a);
  double h2 = r * r - s * s / abs2(d);
  if (h2 < 0) return {};
  if (h2 == 0) return {p};
  pdd h = d / abs(d) * sqrt(h2);
  return {p - h, p + h};
}
