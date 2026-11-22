map<ll, int> cnt; // needs prime(n)
void PollardRho(ll n) {
  if (n == 1) return;
  if (prime(n)) return ++cnt[n], void();
  ll x = 0, y = 0, t = 30, prd = 2, c = 1, q;
  auto f = [&](ll x)
  { return ((__int128)x * x + c) % n; };
  while (t++ % 40 || gcd(prd, n) == 1) {
    if (x == y) x = ++c, y = f(x);
    if ((q = (__int128)prd * abs(x - y) % n)) prd = q;
    x = f(x), y = f(f(y));
  }
  ll d = gcd(prd, n);
  PollardRho(d), PollardRho(n / d);
}
