#include "../prelude.h"
const int kN = 200005;
// main_lorentz calls Zalgo(s) returning a vector; the codebook's Z-value.cpp is
// make_z() filling a global array instead, so the driver supplies this.
vector<int> Zalgo(const string &s) {
  int n = s.size();
  vector<int> z(n);
  z[0] = n;
  for (int i = 1, l = 0, r = 0; i < n; i++) {
    z[i] = max(0, min(r - i + 1, z[i - l]));
    while (i + z[i] < n && s[i + z[i]] == s[z[i]]) z[i]++;
    if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
  }
  return z;
}
#include "../../codebook/5_String/MainLorentz.cpp"

// mod 2^61-1 hash, to check that a candidate's period is minimal
typedef unsigned long long u64;
const u64 HM = (1ULL << 61) - 1;
u64 add(u64 a, u64 b) { a += b; return a >= HM ? a - HM : a; }
u64 mul(u64 a, u64 b) {
  __uint128_t c = (__uint128_t)a * b;
  u64 lo = (u64)(c & HM), hi = (u64)(c >> 61);
  return add(lo, hi);
}
vector<u64> H, PW;
u64 sub(u64 a, u64 b) { return add(a, HM - b); }
u64 get(int l, int r) { return sub(H[r], mul(H[l], PW[r - l])); }  // [l, r)

int main() {
  Waimai;
  string s; cin >> s;
  int n = s.size();
  const u64 B = 131542391;
  H.assign(n + 1, 0); PW.assign(n + 1, 1);
  for (int i = 0; i < n; i++) {
    H[i + 1] = add(mul(H[i], B), (u64)s[i]);
    PW[i + 1] = mul(PW[i], B);
  }
  main_lorentz(s);

  vector<array<int, 3>> runs;
  for (int t = 1; 2 * t <= n; t++) {
    if (rep[t].empty()) continue;
    auto v = rep[t];
    sort(v.begin(), v.end());
    int lo = v[0].F, hi = v[0].S;
    v.pb(INT_MAX, INT_MAX);
    for (size_t i = 1; i < v.size(); i++) {
      if (v[i].F <= hi + 1) { hi = max(hi, v[i].S); continue; }
      runs.pb(array<int, 3>{t, lo, hi + 2 * t});
      lo = v[i].F, hi = v[i].S;
    }
  }
  // keep a run only if t really is the minimal period of s[l, r)
  vector<array<int, 3>> ans;
  for (auto [t, l, r] : runs) {
    bool ok = true;
    for (int q = 2; q * q <= t && ok; q++)
      if (t % q == 0) {
        for (int p : {t / q, q}) {
          if (p < t && get(l, r - p) == get(l + p, r)) { ok = false; break; }
        }
      }
    if (ok && t > 1 && get(l, r - 1) == get(l + 1, r)) ok = false;
    if (ok) ans.pb(array<int, 3>{t, l, r});
  }
  sort(ans.begin(), ans.end()); ans.erase(unique(ans.begin(), ans.end()), ans.end());
  cout << ans.size() << '\n';
  for (auto [t, l, r] : ans) cout << t << ' ' << l << ' ' << r << '\n';
}
