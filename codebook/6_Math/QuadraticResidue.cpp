// p prime; -1 if none
int QuadraticResidue(int a, int p) {
  if (a == 0 || p == 2) return a;
  ll w = 0;
  using P = pair<ll, ll>; // F + S sqrt(w)
  auto mul = [&](P x, P y) -> P {
    return {(x.F * y.F + x.S * y.S % p * w) % p,
            (x.F * y.S + x.S * y.F) % p};
  };
  auto pw = [&](P x, int e) {
    P r(1, 0);
    for (; e; e >>= 1, x = mul(x, x))
      if (e & 1) r = mul(r, x);
    return r.F;
  };
  if (pw({a, 0}, p / 2) != 1) return -1;
  mt19937 rng(random_device{}());
  ll b;
  do b = rng() % p, w = (b * b + p - a) % p;
  while (pw({w, 0}, p / 2) != p - 1);
  return pw({b, 1}, p / 2 + 1);
}
