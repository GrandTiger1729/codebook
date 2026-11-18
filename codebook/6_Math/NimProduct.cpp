// call nim_init() first
typedef unsigned long long ull;
ull nim8[256][256];
bool nim_ready;
ull nim_mul(ull a, ull b, int h = 32) { // a, b < 2^(2h)
  if (a < 2 || b < 2) return a * b;
  if (h == 4 && nim_ready) return nim8[a][b];
  ull m = (1ULL << h) - 1, a0 = a & m, b0 = b & m;
  ull lo = nim_mul(a0, b0, h / 2);
  ull hi = nim_mul(a >> h, b >> h, h / 2);
  ull mid = nim_mul(a0 ^ a >> h, b0 ^ b >> h, h / 2);
  ull sq = nim_mul(hi, 1ULL << (h - 1), h / 2);
  return (mid ^ lo) << h ^ lo ^ sq; // F^2 = F ^ F/2
}
void nim_init() {
  FOR (a, 0, 255) FOR (b, 0, 255)
    nim8[a][b] = nim_mul(a, b, 4);
  nim_ready = true;
}
