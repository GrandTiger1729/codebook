// n < 4,759,123,141      3 : 2, 7, 61
// n < 1,122,004,669,633  4 : 2, 13, 23, 1662803
// n < 3,474,749,660,383  6 : primes <= 13
// n < 2^64             7 :
// 2, 325, 9375, 28178, 450775, 9780504, 1795265022
// 0: n is composite; prime iff all a pass (n >= 2)
bool Miller_Rabin(ll a, ll n) {
  if ((a = a % n) == 0) return 1;
  if (n % 2 == 0) return n == 2;
  int t = __builtin_ctzll(n - 1);
  ll d = (n - 1) >> t, x = 1;
  for (; d; d >>= 1, a = (__int128)a * a % n)
    if (d & 1) x = (__int128)x * a % n;
  if (x == 1 || x == n - 1) return 1;
  while (--t)
    if ((x = (__int128)x * x % n) == n - 1) return 1;
  return 0;
}
