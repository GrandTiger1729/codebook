#define SZ(a) ((int)a.size())
// s ends with a unique minimum 0, other values in [1, K)
// returns sa of size n, sa[0] = n - 1 (the 0); O(n + K)
vector<int> sais(const vector<int> &s, int K) {
  int n = SZ(s);
  vector<int> sa(n), cnt(K + 1), b, pos, id(n), ns;
  if (n == 1) return sa;
  vector<bool> ty(n, true); // S-type: s[i..] < s[i+1..]
  for (int i = n - 2; i >= 0; i--)
    ty[i] = s[i] == s[i + 1] ? ty[i + 1] : s[i] < s[i + 1];
  auto lms = [&](int i) { return i && ty[i] && !ty[i - 1]; };
  for (int x : s) cnt[x + 1]++;
  FOR (c, 0, K - 1) cnt[c + 1] += cnt[c];
  auto induce = [&](const vector<int> &v) {
    sa.assign(n, -1);
    b.assign(cnt.begin() + 1, cnt.end());
    for (int i = SZ(v) - 1; i >= 0; i--)
      sa[--b[s[v[i]]]] = v[i];
    b.assign(cnt.begin(), cnt.end() - 1);
    FOR (i, 0, n - 1) {
      int j = sa[i] - 1;
      if (j >= 0 && !ty[j]) sa[b[s[j]]++] = j;
    }
    b.assign(cnt.begin() + 1, cnt.end());
    for (int i = n - 1; i >= 0; i--) {
      int j = sa[i] - 1;
      if (j >= 0 && ty[j]) sa[--b[s[j]]] = j;
    }
  };
  FOR (i, 1, n - 1) if (lms(i)) pos.pb(i);
  induce(pos);
  auto same = [&](int x, int y) { // equal LMS substrings
    for (int k = 0;; k++) {
      if (s[x + k] != s[y + k]) return false;
      if (k && (lms(x + k) || lms(y + k)))
        return lms(x + k) && lms(y + k);
    }
  };
  int m = 0, last = -1;
  for (int p : sa) if (lms(p)) {
    if (last >= 0 && !same(last, p)) m++;
    id[p] = m, last = p;
  }
  for (int p : pos) ns.pb(id[p]);
  auto nsa = sais(ns, m + 1);
  for (int &k : nsa) k = pos[k];
  induce(nsa);
  return sa;
}
