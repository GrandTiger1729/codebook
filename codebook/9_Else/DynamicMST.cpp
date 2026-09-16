int cnt[maxn], cost[maxn], st[maxn], ed[maxn];
pair<int, int> qr[maxn];
// qr[i].F = id of edge to be changed, qr[i].S = weight
// after operation cnt[i] = number of operation on edge
// i call solve(0, q - 1, v, 0), where v contains edges
// i such that cnt[i] == 0

void contract(int l, int r, vector<int> v, vector<int> &x, vector<int> &y) {
  sort(v.begin(), v.end(), [&](int i, int j) {
      if (cost[i] == cost[j]) return i < j;
      return cost[i] < cost[j];
      });
  djs.save();
  FOR (i, l, r) djs.merge(st[qr[i].F], ed[qr[i].F]);
  FOR (i, 0, (int)v.size() - 1) {
    if (djs.find(st[v[i]]) != djs.find(ed[v[i]])) {
      x.pb(v[i]);
      djs.merge(st[v[i]], ed[v[i]]);
    }
  }
  djs.undo();
  djs.save();
  FOR (i, 0, (int)x.size() - 1)
    djs.merge(st[x[i]], ed[x[i]]);
  FOR (i, 0, (int)v.size() - 1) {
    if (djs.find(st[v[i]]) != djs.find(ed[v[i]])) {
      y.pb(v[i]);
      djs.merge(st[v[i]], ed[v[i]]);
    }
  }
  djs.undo();
}

void solve(int l, int r, vector<int> v, long long c) {
  if (l == r) {
    cost[qr[l].F] = qr[l].S;
    if (st[qr[l].F] == ed[qr[l].F]) {
      printf("%lld\n", c);
      return;
    }
    int minv = qr[l].S;
    FOR (i, 0, (int)v.size() - 1)
      minv = min(minv, cost[v[i]]);
    printf("%lld\n", c + minv);
    return;
  }
  int m = (l + r) >> 1;
  vector<int> lv = v, rv = v;
  vector<int> x, y;
  FOR (i, m + 1, r) {
    cnt[qr[i].F]--;
    if (cnt[qr[i].F] == 0) lv.pb(qr[i].F);
  }
  contract(l, m, lv, x, y);
  long long lc = c, rc = c;
  djs.save();
  FOR (i, 0, (int)x.size() - 1) {
    lc += cost[x[i]];
    djs.merge(st[x[i]], ed[x[i]]);
  }
  solve(l, m, y, lc);
  djs.undo();
  x.clear(), y.clear();
  FOR (i, m + 1, r) cnt[qr[i].F]++;
  FOR (i, l, m) {
    cnt[qr[i].F]--;
    if (cnt[qr[i].F] == 0) rv.pb(qr[i].F);
  }
  contract(m + 1, r, rv, x, y);
  djs.save();
  FOR (i, 0, (int)x.size() - 1) {
    rc += cost[x[i]];
    djs.merge(st[x[i]], ed[x[i]]);
  }
  solve(m + 1, r, y, rc);
  djs.undo();
  FOR (i, l, m) cnt[qr[i].F]++;
}
