#define SZ(a) ((int)a.size())
typedef pair<int, int> pii;
namespace schreier {
int n;
vector<vector<vector<int>>> bkts, binv;
vector<vector<int>> lk;
vector<int> operator*(const vector<int> &a, const vector<int> &b) {
  vector<int> res(SZ(a));
  FOR (i, 0, SZ(a) - 1) res[i] = b[a[i]];
  return res;
}
vector<int> inv(const vector<int> &a) {
  vector<int> res(SZ(a));
  FOR (i, 0, SZ(a) - 1) res[a[i]] = i;
  return res;
}
int filter(const vector<int> &g, bool add = true) {
  n = SZ(bkts);
  vector<int> p = g;
  FOR (i, 0, n - 1) {
    assert(p[i] >= 0 && p[i] < SZ(lk[i]));
    if (lk[i][p[i]] == -1) {
      if (add) {
        bkts[i].pb(p);
        binv[i].pb(inv(p));
        lk[i][p[i]] = SZ(bkts[i]) - 1;
      }
      return i;
    }
    p = p * binv[i][lk[i][p[i]]];
  }
  return -1;
}
bool inside(const vector<int> &g) { return filter(g, false) == -1; }
void solve(const vector<vector<int>> &gen, int _n) {
  n = _n;
  bkts.clear(), bkts.resize(n);
  binv.clear(), binv.resize(n);
  lk.clear(), lk.resize(n);
  vector<int> iden(n);
  iota(iden.begin(), iden.end(), 0);
  FOR (i, 0, n - 1) {
    lk[i].resize(n, -1);
    bkts[i].pb(iden);
    binv[i].pb(iden);
    lk[i][i] = 0;
  }
  FOR (i, 0, SZ(gen) - 1) filter(gen[i]);
  queue<pair<pii, pii>> upd;
  FOR (i, 0, n - 1)
    FOR (j, i, n - 1)
      FOR (k, 0, SZ(bkts[i]) - 1)
        FOR (l, 0, SZ(bkts[j]) - 1)
          upd.emplace(pii(i, k), pii(j, l));
  while (!upd.empty()) {
    auto a = upd.front().F;
    auto b = upd.front().S;
    upd.pop();
    int res = filter(bkts[a.F][a.S] * bkts[b.F][b.S]);
    if (res == -1) continue;
    pii pr = pii(res, SZ(bkts[res]) - 1);
    FOR (i, 0, n - 1)
      FOR (j, 0, SZ(bkts[i]) - 1) {
        if (i <= res) upd.emplace(pii(i, j), pr);
        if (res <= i) upd.emplace(pr, pii(i, j));
      }
  }
}
ll size() {
  ll res = 1;
  FOR (i, 0, n - 1) res = res * SZ(bkts[i]);
  return res;
}}
