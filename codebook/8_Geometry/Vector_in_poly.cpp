// ori(a,b,c) >= 0, valid: "strict" angle from a-b to a-c
bool btwangle(pll a, pll b, pll c, pll p, int strict) {
  return ori(a, b, p) >= strict && ori(a, p, c) >= strict;
}
// vector{cur, p} points inside at cur, ccw prv, cur, nxt
bool inside(pll prv, pll cur, pll nxt, pll p, int st) {
  if (ori(cur, nxt, prv) >= 0)
    return btwangle(cur, nxt, prv, p, st);
  return !btwangle(cur, prv, nxt, p, !st);
}
