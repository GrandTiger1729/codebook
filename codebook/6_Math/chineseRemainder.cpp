ll solve(ll x1, ll m1, ll x2, ll m2) { // lcm < 4e18
  ll g = gcd(m1, m2);
  if ((x2 - x1) % g) return -1; // no sol
  m1 /= g; m2 /= g;
  pair<ll, ll> p = exgcd(m1, m2);
  ll lcm = m1 * m2 * g;
  ll k = (__int128)p.F * ((x2 - x1) / g) % m2;
  ll res = k * m1 * g + x1;
  return (res % lcm + lcm) % lcm;
}
