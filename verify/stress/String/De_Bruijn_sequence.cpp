#include "prelude.h"
#include "stress.h"
#include "5_String/De_Bruijn_sequence.cpp"

// solve(C, N, K, out): out[0..N+K-1) whose K length-N windows are pairwise distinct (K <= C^N);
// for K = C^N, out[0..C^N) is a cyclic de Bruijn sequence.
int out[MAXC * MAXN];
int main() {
  int cases = 0;
  for (int C = 1; C <= 10; C++)
    for (int N = 1; N <= 20; N++) {
      ll tot = 1; bool big = false;
      FOR (i, 1, N) { tot *= C; if (tot > 60000) { big = true; break; } }
      if (big) continue;
      vector<ll> Ks = {tot, 1, tot / 2 + 1, rnd(1, tot), rnd(1, tot)};
      for (ll K : Ks) {
        if (N + K - 1 > MAXC * MAXN) continue;
        fill(out, out + N + K + 5, -7);
        dbs.solve(C, N, K, out);
        int L = N + K - 1;
        assert(out[L] == -7);                       // wrote nothing past L
        FOR (i, 0, L - 1) assert(0 <= out[i] && out[i] < C);
        vector<char> seen(tot, 0);
        for (int i = 0; i + N <= L; i++) {
          ll code = 0; FOR (k, 0, N - 1) code = code * C + out[i + k];
          assert(!seen[code]); seen[code] = 1;
        }
        if (K == tot) {                               // cyclic check on first C^N symbols
          vector<char> seen2(tot, 0);
          FOR (i, 0, tot - 1) {
            ll code = 0; FOR (k, 0, N - 1) code = code * C + out[(i + k) % tot];
            assert(!seen2[code]); seen2[code] = 1;
          }
        }
        cases++;
      }
    }
  // max-size case: C=10, N=5 -> 10^5 + 4 symbols (fits MAXC*MAXN)
  { int C = 10, N = 5, K = 100000; dbs.solve(C, N, K, out);
    vector<char> seen(K, 0);
    for (int i = 0; i < K; i++) { int code = 0; FOR (k, 0, N - 1) code = code * C + out[i + k]; assert(!seen[code]); seen[code] = 1; }
    cases++; }
  printf("De_Bruijn ok: %d (C,N,K) cases, C<=10, C^N<=6e4 plus C=10,N=5 full\n", cases);
}
