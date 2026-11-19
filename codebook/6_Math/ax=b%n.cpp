vector<ll> mod_leq(ll a, ll b, ll n) {
  vector<ll> rt;
  ll x, y, g = exgcd(a, n, x, y);
  if (!(b % g)) {
    x = x * (b / g) % n;
    FOR (i, 0, g - 1)
      rt.pb(x), x = (x + n / g) % n;
  }
  return rt;
}
