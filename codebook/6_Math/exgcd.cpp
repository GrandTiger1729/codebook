// a >= 0, b > 0: ax + by = gcd(a, b), |x| <= b / gcd
pair<ll, ll> exgcd(ll a, ll b) {
  if (!b) return {1, 0};
  auto [x, y] = exgcd(b, a % b);
  return {y, x - a / b * y};
}
