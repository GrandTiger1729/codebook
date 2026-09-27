// The codebook's macro contract. Drivers include this, then the template under test.
#include <bits/stdc++.h>
#include <cassert>   // GCC 16 dropped this from bits/stdc++.h
using namespace std;

#define ll long long
#define Waimai ios::sync_with_stdio(false), cin.tie(0)
#define FOR(x, a, b) for (int x = a, I = b; x <= I; x++)
#define pb emplace_back
#define F first
#define S second

// Anything else (pii/pll/SZ/ALL/X/Y/...) must come from the template itself.
// Geometry drivers include ../geo_prelude.h, i.e. 8_Geometry/Default_code.cpp,
// the chapter prelude that defines X/Y/SZ/ALL/pii/pll.
