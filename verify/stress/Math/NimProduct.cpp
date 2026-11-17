// nim_mul vs the "turning corners" game (SG by brute force, Tartan theorem) for x, y < 32,
// and field axioms on random 64-bit values: commutative, associative, distributive over
// xor, 1 is the identity, and every a != 0 has a^(2^64-1) = 1 (multiplicative group order).
#include "prelude.h"
#include "stress.h"
#include "6_Math/NimProduct.cpp"
int sg[32][32];
int main() {
  nim_init();
  FOR (x, 0, 31) FOR (y, 0, 31) {
    set<int> s;
    FOR (a, 0, x - 1) FOR (b, 0, y - 1) s.insert(sg[a][y] ^ sg[x][b] ^ sg[a][b]);
    int m = 0; while (s.count(m)) m++;
    sg[x][y] = m;
    assert(nim_mul(x, y) == (ull)m);
  }
  int cases = 0;
  auto r64 = [] { return (ull)rng(); };
  auto pw = [](ull a, ull e) { ull r = 1; for (; e; e >>= 1, a = nim_mul(a, a)) if (e & 1) r = nim_mul(r, a); return r; };
  FOR (it, 1, 20000) {
    ull a = r64(), b = r64(), c = r64();
    if (it % 3 == 0) a >>= rnd(0, 63), b >>= rnd(0, 63);
    assert(nim_mul(a, b) == nim_mul(b, a));
    assert(nim_mul(nim_mul(a, b), c) == nim_mul(a, nim_mul(b, c)));
    assert(nim_mul(a, b ^ c) == (nim_mul(a, b) ^ nim_mul(a, c)));
    assert(nim_mul(a, 1) == a && nim_mul(a, 0) == 0);
    if (it % 20 == 0 && a) assert(pw(a, ~0ULL) == 1);
    cases++;
  }
  printf("NimProduct ok: 32x32 turning-corners SG table, %d random field-axiom checks\n", cases);
}
