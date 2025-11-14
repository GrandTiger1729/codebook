// cost(i, j): cost of transit from i to j
// search(i, j): last point when i is better in row i,
// j, need to check whether cost(i, i) is defined
int cost(int i, int j);
int search(int i, int j, int L, int R = N);
void concave1D1D() {
  vector<int> dp(N + 1, 0);
  vector<pii> st; // (idx, right end h_idx)
  st.emplace_back(pii(0, N + 1));
  for (int j = 1; j <= N; j++) {
    while (st.back().S < j) st.pop_back();
    int i = st.back().F;
    dp[j] = dp[i] + cost(i, j);
    while (
      !st.empty() && search(j, i, j) >= st.back().S) {
      st.pop_back();
      i = st.back().F;
    }
    st.emplace_back(pii(j, search(j, i, j)));
  }
}
void convex1D1D() {
  vector<int> dp(N + 1, 0);
  deque<pii> dq; // (idx, right end)
  dq.emplace_back(pii(0, N + 1));
  for (int j = 1; j <= N; j++) {
    while (dq.back().S < j) dq.pop_back();
    int i = dq.back().F;
    dp[j] = dp[i] + cost(i, j);
    while (dq.size() >= 2 &&
      search(i, j, j) <= dq.end()[-2].S) {
      dq.pop_back();
      i = dq.back().F;
    }
    dq.back().S = search(i, j, j);
    dq.emplace_back(pii(j, N + 1));
  }
}