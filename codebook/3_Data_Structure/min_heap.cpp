template <class T, class Info> // lazy add tag
struct min_heap { // mergeable
  using P = pair<T, Info>;
  priority_queue<P, vector<P>, greater<P>> pq;
  T lazy = 0;
  void push(P v) { pq.emplace(v.F - lazy, v.S); }
  P top() { return P(pq.top().F + lazy, pq.top().S); }
  void join(min_heap &rgt) {
    if ((int)pq.size() < (int)rgt.pq.size()) {
      swap(pq, rgt.pq);
      swap(lazy, rgt.lazy);
    }
    while (!rgt.pq.empty()) {
      push(rgt.top());
      rgt.pop();
    }
  }
  void pop() { pq.pop(); }
  bool empty() { return pq.empty(); }
  void add_lazy(T v) { lazy += v; }
};
