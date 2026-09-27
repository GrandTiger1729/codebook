auto globalMinCut(vector<vector<int>> mat) {
  pair<int, vector<int>> best = {INT_MAX, {}};
  int n = mat.size();
  vector<vector<int>> co(n);
  FOR (i, 0, n - 1) co[i] = {i};
  FOR (ph, 1, n - 1) {
    vector<int> w = mat[0];
    int s = 0, t = 0;
    FOR (it, 0, n - ph - 1) {
      w[t] = INT_MIN, s = t;
      t = max_element(w.begin(), w.end()) - w.begin();
      FOR (i, 0, n - 1) w[i] += mat[t][i];
    }
    best = min(best, {w[t] - mat[t][t], co[t]});
    co[s].insert(co[s].end(), co[t].begin(), co[t].end());
    FOR (i, 0, n - 1) mat[s][i] += mat[t][i];
    FOR (i, 0, n - 1) mat[i][s] = mat[s][i];
    mat[0][t] = INT_MIN;
  }
  return best;
} // O(V^3); O(VE log V) with a heap in the inner loop
