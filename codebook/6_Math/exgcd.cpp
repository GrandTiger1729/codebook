// a >= 0, b > 0: |x| <= b / gcd
ll exgcd(ll a, ll b, ll &x, ll &y) { // ax + by = gcd
  if (!b) return x = 1, y = 0, a;
  ll g = exgcd(b, a % b, y, x);
  return y -= a / b * x, g;
}
