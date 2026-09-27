/*
Mo's Algorithm On Tree
Preprocess:
1) LCA
2) dfs with in[u] = dft++, out[u] = dft++
3) ord[in[u]] = ord[out[u]] = u
4) bitset<MAXN> inset
*/
struct Query {
  int L, R, LBid, lca;
  Query(int u, int v) {
    int c = LCA(u, v);
    if (c == u || c == v)
      lca = -1, L = out[c ^ u ^ v], R = out[c];
    else if (out[u] < in[v])
      lca = c, L = out[u], R = in[v];
    else
      lca = c, L = out[v], R = in[u];
    LBid = L / blk;
  }
  bool operator<(const Query &q) const {
    if (LBid != q.LBid) return LBid < q.LBid;
    return R < q.R;
  }
};
void flip(int x) {
  if (inset[x]) sub(arr[x]); // TODO
  else add(arr[x]); // TODO
  inset.flip(x);
}
void solve(vector<Query> query) {
  sort(query.begin(), query.end());
  int L = 0, R = -1;
  for (auto q : query) {
    while (R < q.R) flip(ord[++R]);
    while (L > q.L) flip(ord[--L]);
    while (R > q.R) flip(ord[R--]);
    while (L < q.L) flip(ord[L++]);
    if (~q.lca) add(arr[q.lca]);
    // answer query
    if (~q.lca) sub(arr[q.lca]);
  }
}
