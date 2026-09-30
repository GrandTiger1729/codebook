// simple undirected graph, 0-based, O(M sqrt M)
vector<int> G[N];
int rk[N], vis[N]; // rk: order by (deg, id)
void build(int n) {
  vector<int> p(n);
  iota(p.begin(), p.end(), 0);
  sort(p.begin(), p.end(), [&](int a, int b) {
    return pair(G[a].size(), a) < pair(G[b].size(), b);
  });
  FOR (i, 0, n - 1) rk[p[i]] = i;
}
// calls f(a, b, c) once for every triangle
template <class F> void C3(int n, F f) {
  FOR (a, 0, n - 1) { // rk[a] > rk[b] > rk[c]
    for (int c : G[a]) vis[c] = 1;
    for (int b : G[a]) if (rk[b] < rk[a])
      for (int c : G[b])
        if (rk[c] < rk[b] && vis[c]) f(a, b, c);
    for (int c : G[a]) vis[c] = 0;
  }
}
ll C4(int n) { // number of 4-cycles
  ll ans = 0;
  FOR (a, 0, n - 1) { // a: largest rk on the cycle
    for (int b : G[a]) if (rk[b] < rk[a])
      for (int c : G[b])
        if (rk[c] < rk[a]) ans += vis[c]++;
    for (int b : G[a]) if (rk[b] < rk[a])
      for (int c : G[b]) vis[c] = 0;
  }
  return ans;
}
