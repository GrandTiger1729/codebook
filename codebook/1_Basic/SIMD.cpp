#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt") // avx512vl
#include <immintrin.h> // pragmas MUST precede #include
// GCC vec: lane-wise ops, cmp true=-1, scalar broadcast, v[i]
typedef int v8i __attribute__((vector_size(32))); // 8i/4ll/32c
v8i ld(const int *p) { v8i v; memcpy(&v, p, 32); return v; }
void st(int *p, v8i v) { memcpy(p, &v, 32); }
void add(int *a, int l, int r, int x) { // [l, r)
  for (; l + 8 <= r; l += 8) st(a + l, ld(a + l) + x);
  for (; l < r; l++) a[l] += x;
}
int cnt_lt(const int *a, int n, int y) {
  v8i c{}; int i = 0, r = 0;
  for (; i + 8 <= n; i += 8) c -= ld(a + i) < y;
  FOR (j, 0, 7) r += c[j];
  for (; i < n; i++) r += a[i] < y;
  return r;
}
alignas(32) int a[N]; __m256i *p = (__m256i*)a;
// _mm256_<op>_<epi8|16|32|64|epu8..|ps|pd>
// 128b: __m128{i,,d}, _mm_, _si128 | 8b: char + epi8 (32/vec)
// Init: setzero_si256, set1, set(hi..lo!), setr(lo..hi)
// Mem:  loadu/storeu_si256, insert_epi32, extract_epi32/64
// Math: add sub mullo(32b) mul_epi32(even->64b) min max abs
//       madd_epi16, sad_epu8 (epi64 min/max/mullo: AVX512)
// Cmp:  cmpgt cmpeq (AVX2 has no cmplt -> cmpgt(b,a))
// Bit:  and/or/xor/andnot((~a)&b), testz_si256(a,a): a==0
//       blendv_epi8(a,b,m)=m?b:a, movemask_epi8/ps(8msb)
// Cast: castsi256_ps, cvtepi32_epi64(__m128i->256i)
// Lane (AVX2 splits 2x128b!): shuffle_epi32(a,imm8)
//       slli/srli_si256(a,bytes), alignr_epi8(a,b,bytes):[b,a]
// Xlane: permutevar8x32_epi32, i32gather_epi32(base,idx,4)
// 128b prefix op(add/max): a=op(a,slli_si128(a,4)); op(...,8);