int z[SIZE];
void make_z(const string &s) {
  int l = 0, r = 0, n = s.size();
  FOR (i, 1, n - 1) {
    for (z[i] = max(0, min(r - i + 1, z[i - l]));
         i + z[i] < n && s[i + z[i]] == s[z[i]];
         ++z[i])
      ;
    if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
  }
}
