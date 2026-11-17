// polynomial hash mod 2^61 - 1 with a random base; two
// different strings collide w.p. ~ n / 2^61. s[i] >= 0
typedef unsigned long long ull;
const ull M = (1ULL << 61) - 1;
mt19937_64 hrng(
  chrono::steady_clock::now().time_since_epoch().count());
const ull B = hrng() % (M - 256) + 256;
ull hmul(ull a, ull b) {
  __uint128_t c = (__uint128_t)a * b;
  ull r = (ull)(c & M) + (ull)(c >> 61);
  return r >= M ? r - M : r;
}
struct Hash {
  vector<ull> h, p;
  Hash(const auto &s)
    : h(s.size() + 1), p(s.size() + 1, 1) {
    FOR (i, 0, (int)s.size() - 1) {
      h[i + 1] = hmul(h[i], B) + (ull)s[i] % M;
      if (h[i + 1] >= M) h[i + 1] -= M;
      p[i + 1] = hmul(p[i], B);
    }
  }
  ull get(int l, int r) { // s[l, r)
    ull x = h[r] + M - hmul(h[l], p[r - l]);
    return x >= M ? x - M : x;
  }
};
