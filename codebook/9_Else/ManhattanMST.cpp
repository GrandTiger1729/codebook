void solve(Point *a, int n) {
    sort(a, a + n, [](const Point &p, const Point &q) {
        return p.x + p.y < q.x + q.y;
    });
    set<Point> st; // greater<Point::x>
    FOR (i, 0, n - 1) {
      for (auto it = st.lower_bound(a[i]);
        it != st.end(); it = st.erase(it)) {
        if (it->x - it->y < a[i].x - a[i].y) break;
        es.pb(it->u, a[i].u, dist(*it, a[i]));
      }
      st.insert(a[i]);
    }
}
void MST(Point *a, int n) {
  FOR (t, 0, 1) {
    solve(a, n);
    FOR (i, 0, n - 1) swap(a[i].x, a[i].y);
    solve(a, n);
    FOR (i, 0, n - 1) a[i].x = -a[i].x;
  }
}
