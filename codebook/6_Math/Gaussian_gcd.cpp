typedef complex<__int128_t> cpx;
cpx gaussian_gcd(cpx a, cpx b) {
#define rnd(a, b) ((2 * a + (a < 0 ? -b : b)) / (2 * b))
  auto c = a.real() * b.real() + a.imag() * b.imag();
  auto d = a.imag() * b.real() - a.real() * b.imag();
  auto r = b.real() * b.real() + b.imag() * b.imag();
  if (c % r == 0 && d % r == 0) return b;
  return gaussian_gcd(b,
    a - cpx(rnd(c, r), rnd(d, r)) * b);
}
