#pragma GCC target("avx2") // before #include; or sse4.2
#include <immintrin.h>
// GCC vector extension: + - * / % & | ^ ~ << >> ?: and
// == < etc. are lane-wise (true = -1); a scalar operand
// is broadcast; v[i] is lane i. ll: vector_size(32) = 4
typedef int v8i __attribute__((vector_size(32)));
v8i ld(const int *p)
{ v8i v; memcpy(&v, p, 32); return v; }
void st(int *p, v8i v) { memcpy(p, &v, 32); }
// e.g. brute force "a[l..r] += x" and "count a[i] < y"
void add(int *a, int l, int r, int x) { // [l, r)
  for (; l + 8 <= r; l += 8) st(a + l, ld(a + l) + x);
  for (; l < r; l++) a[l] += x;
}
int count_less(const int *a, int n, int y) {
  v8i c{};
  int i = 0, res = 0;
  for (; i + 8 <= n; i += 8) c -= ld(a + i) < y;
  FOR (j, 0, 7) res += c[j];
  for (; i < n; i++) res += a[i] < y;
  return res;
}
// intrinsics: _mm256_<op>_<epi8|epi16|epi32|epi64|ps|pd>
// loadu/storeu_si256, set1, setzero_si256, add, sub,
// mullo, min, max, abs, cmpgt, cmpeq, and/or/xor/andnot
// (no min/max/mullo for epi64 before AVX-512)
// blendv_epi8(a, b, mask) = mask ? b : a, movemask_epi8
// madd_epi16: pairwise i16 dot -> 8 x i32; sad_epu8
// i32gather_epi32(base, idx, 4); permutevar8x32_epi32
// _mm256_testz_si256(a, a): all zero; popcount via
// __builtin_popcountll on 64-bit lanes (no AVX2 vpopcnt)
