ll exgcd(ll a, ll b, ll &x, ll &y) {
  if (!b) return x = 1, y = 0, a;
  ll g = exgcd(b, a % b, y, x);
  return y -= a / b * x, g;
}
// ax=c (mod m), g | c: x*(c/g) mod m/g, period m/g
