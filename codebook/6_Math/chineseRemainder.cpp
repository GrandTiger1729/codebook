ll crt(ll x1, ll m1, ll x2, ll m2) { // lcm < 4e18
  ll p, q, g = exgcd(m1, m2, p, q);
  if ((x2 - x1) % g) return -1; // no sol
  m2 /= g;
  ll k = (__int128)p * ((x2 - x1) / g) % m2;
  ll lcm = m1 * m2, res = k * m1 + x1;
  return (res % lcm + lcm) % lcm;
}
