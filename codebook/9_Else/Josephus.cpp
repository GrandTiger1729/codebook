// survivor (0-base) of n people, every k-th dies
ll josephus(ll n, ll k) { // O(min(n, k log n))
  if (n == 1) return 0;
  if (k == 1) return n - 1;
  if (k > n) return (josephus(n - 1, k) + k) % n;
  ll c = n / k, r = josephus(n - c, k) - n % k;
  return r < 0 ? r + n : r + r / (k - 1);
}
