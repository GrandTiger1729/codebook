typedef unsigned long long ull; // need G, seed
ull seed, h[N]; // h[u]: hash of u's subtree
ull shift(ull x) {
  x ^= x << 13;
  x ^= x >> 7;
  x ^= x << 17;
  return x;
}
ull dfs(int u, int f) {
  ull sum = seed;
  for (int i : G[u])
    if (i != f)
      sum += shift(dfs(i, u));
  return h[u] = sum;
}
