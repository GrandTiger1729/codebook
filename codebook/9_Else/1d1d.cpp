// dp[i] = min_{j<i} dp[j] + w(j, i), set dp[0] first
// convex:  w(a,c)+w(b,d) <= w(a,d)+w(b,c) for a<b<c<d,
//          a newer j wins the columns to the right
// concave: the reverse inequality, it wins to the left
ll w(int j, int i);
ll dp[N];
struct Seg { int p, l, r; }; // dp[l..r] best from p
ll cal(int j, int i) { return dp[j] + w(j, i); }
void convex1D1D(int n) {
  deque<Seg> dq{{0, 1, n}};
  FOR (i, 1, n) {
    dp[i] = cal(dq[0].p, i);
    if (++dq[0].l > dq[0].r) dq.pop_front();
    while (!dq.empty()) { // i wins a whole segment
      auto [p, l, r] = dq.back();
      if (cal(i, l) > cal(p, l)) break;
      dq.pop_back();
    }
    int st = i + 1; // i wins columns st..n
    if (!dq.empty()) {
      auto &[p, l, r] = dq.back();
      st = r + 1;
      if (cal(i, r) < cal(p, r)) { // binary search
        int lo = l, hi = r;
        while (lo < hi) {
          int m = (lo + hi) / 2;
          if (cal(i, m) < cal(p, m)) hi = m;
          else lo = m + 1;
        }
        st = lo;
      }
      r = st - 1;
    }
    if (st <= n) dq.push_back({i, st, n});
  }
}
void concave1D1D(int n) {
  deque<Seg> dq{{0, 1, n}};
  FOR (i, 1, n) {
    dp[i] = cal(dq[0].p, i);
    if (++dq[0].l > dq[0].r) dq.pop_front();
    while (!dq.empty()) { // i wins a whole segment
      auto [p, l, r] = dq[0];
      if (cal(i, r) > cal(p, r)) break;
      dq.pop_front();
    }
    int ed = n; // i wins columns i+1..ed
    if (!dq.empty()) {
      auto &[p, l, r] = dq[0];
      ed = l - 1;
      if (cal(i, l) < cal(p, l)) { // binary search
        int lo = l, hi = r;
        while (lo < hi) {
          int m = (lo + hi + 1) / 2;
          if (cal(i, m) < cal(p, m)) lo = m;
          else hi = m - 1;
        }
        ed = lo;
      }
      l = ed + 1;
    }
    if (ed > i) dq.push_front({i, i + 1, ed});
  }
}
