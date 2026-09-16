#define L 0
#define LU 1
#define U 2
const int mov[3][2] = {0, -1, -1, -1, -1, 0};
int al, bl;
char a[MAXL * 2], b[MAXL * 2]; // 0-indexed
int dp[MAXL * 2][MAXL];
char pred[MAXL * 2][MAXL];
inline int lcs_length(int r) {
  int i = r + al, j = bl, l = 0;
  while (i > r) {
    char dir = pred[i][j];
    if (dir == LU) l++;
    i += mov[dir][0];
    j += mov[dir][1];
  }
  return l;
}
inline void reroot(int r) { // r = new base row
  int i = r, j = 1;
  while (j <= bl && pred[i][j] != LU) j++;
  if (j > bl) return;
  pred[i][j] = L;
  while (i < 2 * al && j <= bl) {
    if (pred[i + 1][j] == U) {
      i++;
      pred[i][j] = L;
    } else if (j < bl && pred[i + 1][j + 1] == LU) {
      i++;
      j++;
      pred[i][j] = L;
    } else {
      j++;
    }
  }
}
int cyclic_lcs() {
  // a, b, al, bl should be properly filled
  // note: a WILL be altered in process
  //        -- concatenated after itself
  char tmp[MAXL];
  if (al > bl) {
    swap(al, bl);
    strcpy(tmp, a);
    strcpy(a, b);
    strcpy(b, tmp);
  }
  strcpy(tmp, a);
  strcat(a, tmp);
  // basic lcs
  FOR (i, 0, 2 * al) {
    dp[i][0] = 0;
    pred[i][0] = U;
  }
  FOR (j, 0, bl) {
    dp[0][j] = 0;
    pred[0][j] = L;
  }
  FOR (i, 1, 2 * al) {
    FOR (j, 1, bl) {
      if (a[i - 1] == b[j - 1])
        dp[i][j] = dp[i - 1][j - 1] + 1;
      else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
      if (dp[i][j - 1] == dp[i][j]) pred[i][j] = L;
      else if (a[i - 1] == b[j - 1]) pred[i][j] = LU;
      else pred[i][j] = U;
    }
  }
  // do cyclic lcs
  int clcs = 0;
  FOR (i, 0, al - 1) {
    clcs = max(clcs, lcs_length(i));
    reroot(i + 1);
  }
  // recover a
  a[al] = '\0';
  return clcs;
}
