// Shared helpers for stress tests. Include after prelude.h (and geo_prelude.h for geometry).
#pragma once
mt19937_64 rng(1729);
ll rnd(ll lo, ll hi) { return uniform_int_distribution<ll>(lo, hi)(rng); } // inclusive
double rndd(double lo, double hi) { return uniform_real_distribution<double>(lo, hi)(rng); }
