pdd circenter(pdd a, pdd b, pdd c) { // r = abs(a - ret)
  b = b - a, c = c - a;
  return a + perp(b * abs2(c) - c * abs2(b))
    / cross(b, c) / 2;
}
pdd incenter(pdd p1, pdd p2, pdd p3) { // r = area*2/s
  double a = abs(p2 - p3), b = abs(p1 - p3),
    c = abs(p1 - p2), s = a + b + c;
  return (p1 * a + p2 * b + p3 * c) / s;
}
pdd masscenter(pdd p1, pdd p2, pdd p3)
{ return (p1 + p2 + p3) / 3; }
pdd orthcenter(pdd p1, pdd p2, pdd p3)
{ return p1 + p2 + p3 - circenter(p1, p2, p3) * 2; }
