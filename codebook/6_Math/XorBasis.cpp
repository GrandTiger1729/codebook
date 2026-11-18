// max_xor(l): only values inserted at time >= l
// (copy per prefix r for range a[l..r])
struct XorBasis {
  ll b[63] = {};
  int t[63] = {};
  void insert(ll x, int tm = 0) {
    for (int i = 62; i >= 0; i--) if (x >> i & 1) {
      if (!b[i]) return b[i] = x, t[i] = tm, void();
      if (t[i] < tm) swap(b[i], x), swap(t[i], tm);
      x ^= b[i];
    }
  }
  ll max_xor(int l = 0, ll x = 0) {
    for (int i = 62; i >= 0; i--)
      if (b[i] && t[i] >= l) x = max(x, x ^ b[i]);
    return x;
  }
  ll kth(ll k) { // k-th smallest (0-base) of the span
    vector<ll> v; // -1 if k >= size of the span
    FOR (i, 0, 62) if (b[i]) {
      ll x = b[i];
      for (ll y : v) if (x >> __lg(y) & 1) x ^= y;
      v.pb(x);
    }
    if (k >> v.size()) return -1;
    ll res = 0;
    FOR (i, 0, (int)v.size() - 1)
      if (k >> i & 1) res ^= v[i];
    return res;
  }
};
