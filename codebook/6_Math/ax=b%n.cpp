vector<ll> mod_leq(ll a, ll b, ll n) {
  vector<ll> rt;
  ll g = gcd(a, n);
  pair<ll, ll> p = exgcd(a, n);
  if (!(b % g)) {
    p.F = p.F * (b / g) % n;
    FOR (i, 0, g - 1)
      rt.pb(p.F), p.F = (p.F + n / g) % n;
  }
  return rt;
}
