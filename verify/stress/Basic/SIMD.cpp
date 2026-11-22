// SIMD.cpp: add() and cnt_lt() vs scalar loops on random arrays/ranges (all
// alignments and tails), plus every intrinsic named in the cheat sheet: each is called
// (a misspelt name fails to compile) and its claimed meaning (set order, lane-local
// shifts, alignr order, prefix trick, ...) is checked on known inputs. Prints the
// speedup of cnt_lt.
#include "prelude.h"
const int N = 64; // the sheet's aligned buffer
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
    assert(cnt_lt(a.data(), n, y) == want);
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
  // the rest of the sheet, each claim checked on a known input
  auto get = [](__m256i x) { array<int, 8> r; _mm256_storeu_si256((__m256i *)r.data(), x); return r; };
  auto iota8 = [](int s) { array<int, 8> r; FOR (i, 0, 7) r[i] = s + i; return r; };
  // set(hi..lo!), setr(lo..hi): both give lanes 0..7 here
  assert(get(_mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0)) == iota8(0));
  assert(get(_mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7)) == iota8(0));
  __m256i I = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
  // insert / extract
  assert(get(_mm256_insert_epi32(I, 42, 5))[5] == 42 && _mm256_extract_epi32(I, 6) == 6);
  assert(_mm256_extract_epi64(I, 1) == ((ll)3 << 32 | 2));
  // mul_epi32: even lanes (signed) -> 64-bit products
  {
    __m256i X = _mm256_setr_epi32(-3, 99, 100000, 99, 7, 99, -70000, 99);
    __m256i Y = _mm256_setr_epi32(5, 99, 300000, 99, -7, 99, 80000, 99);
    ll r[4]; _mm256_storeu_si256((__m256i *)r, _mm256_mul_epi32(X, Y));
    assert(r[0] == -15 && r[1] == 30000000000LL && r[2] == -49 && r[3] == -5600000000LL);
  }
  // castsi256_ps: bits unchanged
  {
    float f[8]; _mm256_storeu_ps(f, _mm256_castsi256_ps(_mm256_set1_epi32(0x3f800000)));
    assert(f[0] == 1.0f && f[7] == 1.0f);
  }
  // cvtepi32_epi64: 4 ints of a __m128i -> 4 long longs (sign-extended)
  {
    ll r[4]; _mm256_storeu_si256((__m256i *)r, _mm256_cvtepi32_epi64(_mm_setr_epi32(-1, 2, -3, 4)));
    assert(r[0] == -1 && r[1] == 2 && r[2] == -3 && r[3] == 4);
  }
  // movemask_ps: top bit of each 32-bit lane
  assert(_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_setr_epi32(-1, 0, -1, 0, 0, 0, 0, -1))) == 0b10000101);
  // lane-local (AVX2 = two 128-bit halves): shuffle, byte shifts, alignr
  {
    auto r = get(_mm256_shuffle_epi32(I, 0x1b)); // reverse within each half
    assert((r == array<int, 8>{3, 2, 1, 0, 7, 6, 5, 4}));
    r = get(_mm256_slli_si256(I, 4)); // left by 4 bytes per half: zero enters each half
    assert((r == array<int, 8>{0, 0, 1, 2, 0, 4, 5, 6}));
    r = get(_mm256_srli_si256(I, 4));
    assert((r == array<int, 8>{1, 2, 3, 0, 5, 6, 7, 0}));
    __m256i H = _mm256_setr_epi32(10, 11, 12, 13, 14, 15, 16, 17);
    r = get(_mm256_alignr_epi8(H, I, 4)); // per half: (a high, b low) >> 4 bytes = [b, a] shifted
    assert((r == array<int, 8>{1, 2, 3, 10, 5, 6, 7, 14}));
  }
  // permutevar8x32 crosses halves (unlike shuffle)
  assert((get(_mm256_permutevar8x32_epi32(I, _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0))) == array<int, 8>{7, 6, 5, 4, 3, 2, 1, 0}));
  // gather: base[idx[i]], scale 4 = bytes per int
  {
    int base[16]; FOR (i, 0, 15) base[i] = 100 + i;
    assert((get(_mm256_i32gather_epi32(base, _mm256_setr_epi32(0, 15, 3, 3, 8, 1, 2, 9), 4)) == array<int, 8>{100, 115, 103, 103, 108, 101, 102, 109}));
  }
  // cmpgt(b, a) as cmplt(a, b)
  assert((get(_mm256_cmpgt_epi32(_mm256_set1_epi32(4), I)) == array<int, 8>{-1, -1, -1, -1, 0, 0, 0, 0}));
  // 128-bit prefix trick: a = op(a, slli_si128(a, 4)), then 8
  FOR (rep, 1, 1000) {
    int v[4]; FOR (i, 0, 3) v[i] = rnd(0, 1000);
    __m128i x = _mm_loadu_si128((__m128i *)v);
    __m128i s = _mm_add_epi32(x, _mm_slli_si128(x, 4)); s = _mm_add_epi32(s, _mm_slli_si128(s, 8));
    __m128i m = _mm_max_epi32(x, _mm_slli_si128(x, 4)); m = _mm_max_epi32(m, _mm_slli_si128(m, 8));
    int rs[4], rm[4]; _mm_storeu_si128((__m128i *)rs, s), _mm_storeu_si128((__m128i *)rm, m);
    int ps = 0, pm = 0;
    FOR (i, 0, 3) ps += v[i], pm = max(pm, v[i]), assert(rs[i] == ps && rm[i] == pm);
  }
  // speed (informational)
  int n = 1 << 16; vector<int> v(n); for (int &x : v) x = rnd(0, 1000);
  auto t0 = chrono::steady_clock::now(); ll s1 = 0;
  FOR (r, 1, 2000) s1 += cnt_lt(v.data(), n, r % 1000);
  auto t1 = chrono::steady_clock::now(); ll s2 = 0;
  FOR (r, 1, 2000) { int y = r % 1000; for (int x : v) s2 += x < y; asm volatile("" ::: "memory"); }
  auto t2 = chrono::steady_clock::now();
  assert(s1 == s2);
  printf("SIMD ok: %d cases; cnt_lt %.3fs vs scalar %.3fs\n", cases,
         chrono::duration<double>(t1 - t0).count(), chrono::duration<double>(t2 - t1).count());
}
