// n people 0..n-1 in a circle, every k-th one dies;
// returns the survivor. O(n): s = (s + k) % i, i = 2..n
ll josephus(ll n, ll k) { // O(min(n, k log n))
  if (n == 1) return 0;
  if (k == 1) return n - 1;
  if (k > n) return (josephus(n - 1, k) + k) % n;
  ll c = n / k, r = josephus(n - c, k) - n % k;
  return r < 0 ? r + n : r + r / (k - 1);
}
