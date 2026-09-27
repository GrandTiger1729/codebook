template <class T, class Info> // leftist tree, lazy add
struct min_heap { // push, pop, join: O(log n)
  struct node {
    T key, lazy = 0;
    Info info;
    int dist = 1; // length of the right spine
    node *l = 0, *r = 0;
    node(T k, Info i) : key(k), info(i) {}
  };
  node *root = 0;
  static void apply(node *x, T v) {
    if (x) x->key += v, x->lazy += v;
  }
  static int dist(node *x) { return x ? x->dist : 0; }
  static node *merge(node *a, node *b) {
    if (!a || !b) return a ? a : b;
    if (b->key < a->key) swap(a, b);
    apply(a->l, a->lazy), apply(a->r, a->lazy);
    a->lazy = 0;
    a->r = merge(a->r, b);
    if (dist(a->l) < dist(a->r)) swap(a->l, a->r);
    a->dist = dist(a->r) + 1;
    return a;
  }
  void push(pair<T, Info> p) {
    root = merge(root, new node(p.F, p.S));
  }
  pair<T, Info> top() { return {root->key, root->info}; }
  void pop() {
    node *x = root;
    apply(x->l, x->lazy), apply(x->r, x->lazy);
    root = merge(x->l, x->r);
    delete x;
  }
  void join(min_heap &o) {
    root = merge(root, o.root), o.root = 0;
  }
  bool empty() { return !root; }
  void add_lazy(T v) { apply(root, v); }
};
