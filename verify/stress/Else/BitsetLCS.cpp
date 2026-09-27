#include "prelude.h"
#include "stress.h"

// The snippet needs a user-supplied bitset with subtraction and a 1-bit left shift.
const int MAXLEN = 300, W = (MAXLEN + 64) / 64 + 1, SIGMA = 10;
struct Bits {
  unsigned long long a[W] = {};
  void set(int i) { a[i >> 6] |= 1ULL << (i & 63); }
  Bits &operator|=(const Bits &o) { FOR (i, 0, W - 1) a[i] |= o.a[i]; return *this; }
  Bits &operator^=(const Bits &o) { FOR (i, 0, W - 1) a[i] ^= o.a[i]; return *this; }
  Bits &operator&=(const Bits &o) { FOR (i, 0, W - 1) a[i] &= o.a[i]; return *this; }
  Bits operator-(const Bits &o) const {
    Bits r; unsigned long long br = 0;
    FOR (i, 0, W - 1) { unsigned long long x = a[i], y = o.a[i];
      r.a[i] = x - y - br; br = (x < y) || (x - y < br); }
    return r;
  }
  void shiftLeftByOne() { for (int i = W - 1; i >= 0; i--) a[i] = a[i] << 1 | (i ? a[i - 1] >> 63 : 0); }
  int count() const { int c = 0; FOR (i, 0, W - 1) c += __builtin_popcountll(a[i]); return c; }
};
int n, m; Bits p[SIGMA], f, g;

void run() {
  n = m = 0; FOR (i, 0, SIGMA - 1) p[i] = Bits(); f = g = Bits();
#include "9_Else/BitsetLCS.cpp"
}

int lcs(const vector<int> &A, const vector<int> &B) {
  vector<vector<int>> dp(A.size() + 1, vector<int>(B.size() + 1));
  FOR (i, 1, (int)A.size()) FOR (j, 1, (int)B.size())
    dp[i][j] = A[i - 1] == B[j - 1] ? dp[i - 1][j - 1] + 1 : max(dp[i - 1][j], dp[i][j - 1]);
  return dp[A.size()][B.size()];
}
int main() {
  int cases = 0;
  for (int it = 0; it < 10000; it++) {
    int sig = rnd(1, SIGMA), la = rnd(1, it % 20 ? 70 : MAXLEN), lb = rnd(1, it % 20 ? 70 : MAXLEN);
    vector<int> A(la), B(lb);
    for (auto &x : A) x = rnd(0, sig - 1);
    for (auto &x : B) x = rnd(0, sig - 1);
    stringstream in, out;
    in << la << ' ' << lb << '\n'; for (int x : A) in << x << ' '; for (int x : B) in << x << ' ';
    auto *oi = cin.rdbuf(in.rdbuf()); auto *oo = cout.rdbuf(out.rdbuf());
    run();
    cin.rdbuf(oi); cout.rdbuf(oo);
    int got; out >> got;
    assert(got == lcs(A, B));
    cases++;
  }
  printf("BitsetLCS ok: %d cases (n,m<=%d, sigma<=%d, multi-word borrow)\n", cases, MAXLEN, SIGMA);
}
