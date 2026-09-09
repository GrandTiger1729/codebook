int fail[MAXN];
vector<int> match(string A, string B) {
  vector<int> ans;
  fail[0] = -1, fail[1] = 0;
  for (int i = 1, j = 0; i < (int)B.size();
    fail[++i] = ++j) {
    if (B[i] == B[j]) fail[i] = fail[j]; // optimize
    while (j != -1 && B[i] != B[j]) j = fail[j];
  }
  for (int i = 0, j = 0; i < (int)A.size(); i++) {
    while (j != -1 && A[i] != B[j]) j = fail[j];
    if (++j == (int)B.size())
      ans.pb(i + 1 - j), j = fail[j];
  }
  return ans;
}
