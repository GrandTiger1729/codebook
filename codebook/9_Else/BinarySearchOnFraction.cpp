struct Q {
  ll p, q;
  Q go(Q b, ll k) { return {p + b.p * k, q + b.q * k}; }
};
bool pred(Q); // 0..0 1..1, e.g. a.p * y >= x * a.q
Q frac_bs(ll N) { // min p/q with pred, 0 <= p, q <= N
  Q lo{0, 1}, hi{1, 0}; // pred(lo) = 0, pred(hi) = 1
  if (pred(lo)) return lo;
  for (int r = 0; ; r++) { // move hi, lo, hi, ...
    bool d = r & 1;
    Q &a = d ? lo : hi, b = d ? hi : lo;
    auto ok = [&](ll k) { // k = 1: failed last round
      Q m = a.go(b, k);
      return m.p <= N && m.q <= N &&
        ((r && k == 1) || pred(m) != d);
    };
    ll len = 0, step = 1;
    while (ok(len + step)) len += step, step *= 2;
    for (step /= 2; step; step /= 2)
      if (ok(len + step)) len += step;
    if (r && !len) return hi;
    a = a.go(b, len);
  }
}
