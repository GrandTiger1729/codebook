#pragma GCC target("avx2") // before #include; or sse4.2
#include <immintrin.h>
// ops are lane-wise (compare: true = -1), a scalar is
// broadcast, v[i] = lane i; ll: vector_size(32) = 4
typedef int v8i __attribute__((vector_size(32)));
v8i ld(const int *p)
{ v8i v; memcpy(&v, p, 32); return v; }
void st(int *p, v8i v) { memcpy(p, &v, 32); }
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
// _mm256_<op>_<epi8|16|32|64|ps|pd>: loadu/storeu_si256
// set1 setzero_si256 add sub mullo min max abs cmpgt
// cmpeq and/or/xor/andnot (epi64 min/max/mullo: AVX512)
// blendv_epi8(a, b, m) = m ? b : a, movemask_epi8,
// madd_epi16, sad_epu8, i32gather_epi32(base, idx, 4),
// permutevar8x32_epi32, testz_si256(a, a): a == 0
