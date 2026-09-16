MaxFlow Dinic;
int g[MAXN]; 
void GomoryHu(int n) { // 0-base
  fill_n(g, n, 0);
  FOR (i, 1, n - 1) {
    Dinic.reset();
    add_edge(i, g[i], Dinic.maxflow(i, g[i]));
    FOR (j, i + 1, n)
      if (g[j] == g[i] && ~Dinic.dis[j])
        g[j] = i;
  }
}
