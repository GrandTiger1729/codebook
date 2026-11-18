// SIMD.cpp: add() and count_less() vs scalar loops on random arrays/ranges (all
// alignments and tails), plus every intrinsic named in the cheat sheet, each checked
// on one input so a misspelt name fails to compile. Prints the speedup of count_less.
#include "prelude.h"
#include "1_Basic/SIMD.cpp"
#include "stress.h"
int main() {
  int cases = 0;
  FOR (it, 1, 20000) {
    int n = rnd(0, 100);
    vector<int> a(n), b;
    for (int &x : a) x = rnd(-1e9, 1e9) / (it % 2 ? 1 : 100000000);
    b = a;
    int l = rnd(0, n), r = rnd(l, n), x = rnd(-5, 5), y = rnd(-12, 12);
    add(a.data(), l, r, x);
    FOR (i, l, r - 1) b[i] += x;
    assert(a == b);
    int want = 0; for (int v : b) want += v < y;
    assert(count_less(a.data(), n, y) == want);
    cases++;
  }
  // the cheat sheet's intrinsics
  alignas(32) int p[8], q[8];
  FOR (i, 0, 7) p[i] = i - 3, q[i] = 2 * i;
  __m256i A = _mm256_loadu_si256((__m256i *)p), B = _mm256_loadu_si256((__m256i *)q);
  __m256i Z = _mm256_setzero_si256(), S = _mm256_set1_epi32(5);
  _mm256_storeu_si256((__m256i *)p, _mm256_add_epi32(A, S));
  assert(p[0] == 2 && p[7] == 9);
  A = _mm256_loadu_si256((__m256i *)p);
  __m256i M = _mm256_cmpgt_epi32(A, B); // p > q?
  int mm = _mm256_movemask_epi8(M), blend[8];
  _mm256_storeu_si256((__m256i *)blend, _mm256_blendv_epi8(A, B, M));
  FOR (i, 0, 7) assert(blend[i] == (p[i] > q[i] ? q[i] : p[i]) && ((mm >> 4 * i & 15) == 15) == (p[i] > q[i]));
  (void)_mm256_sub_epi32(A, B), (void)_mm256_mullo_epi32(A, B), (void)_mm256_min_epi32(A, B);
  (void)_mm256_max_epi32(A, B), (void)_mm256_abs_epi32(A), (void)_mm256_cmpeq_epi32(A, B);
  (void)_mm256_and_si256(A, B), (void)_mm256_or_si256(A, B), (void)_mm256_xor_si256(A, B);
  (void)_mm256_andnot_si256(A, B), (void)_mm256_madd_epi16(A, B), (void)_mm256_sad_epu8(A, B);
  (void)_mm256_i32gather_epi32(q, _mm256_set1_epi32(3), 4), (void)_mm256_permutevar8x32_epi32(A, B);
  assert(_mm256_testz_si256(Z, Z) && !_mm256_testz_si256(A, A));
  // speed (informational)
  int n = 1 << 16; vector<int> v(n); for (int &x : v) x = rnd(0, 1000);
  auto t0 = chrono::steady_clock::now(); ll s1 = 0;
  FOR (r, 1, 2000) s1 += count_less(v.data(), n, r % 1000);
  auto t1 = chrono::steady_clock::now(); ll s2 = 0;
  FOR (r, 1, 2000) { int y = r % 1000; for (int x : v) s2 += x < y; asm volatile("" ::: "memory"); }
  auto t2 = chrono::steady_clock::now();
  assert(s1 == s2);
  printf("SIMD ok: %d cases; count_less %.3fs vs scalar %.3fs\n", cases,
         chrono::duration<double>(t1 - t0).count(), chrono::duration<double>(t2 - t1).count());
}
