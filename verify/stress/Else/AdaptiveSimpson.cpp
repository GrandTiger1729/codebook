#include "prelude.h"
#include "stress.h"
#include "9_Else/AdaptiveSimpson.cpp"

int main() {
  int cases = 0, misses = 0;
  // cubic polynomials: Simpson is exact
  for (int it = 0; it < 20000; it++) {
    double a = rndd(-5, 5), b = rndd(-5, 5), c = rndd(-5, 5), e = rndd(-5, 5);
    double l = rndd(-10, 10), r = l + rndd(0, 10);
    auto F = [&](double x) { return ((a / 4 * x + b / 3) * x + c / 2) * x * x + e * x; };
    auto S = make_simpson([&](double x) { return ((a * x + b) * x + c) * x + e; });
    double want = F(r) - F(l);
    assert(abs(S.eval(l, r, 1e-9) - want) <= 1e-7 * max(1.0, abs(want)));
    assert(abs(S.eval2(l, r, 1e-9, 7) - want) <= 1e-7 * max(1.0, abs(want)));
    cases++;
  }
  // smooth non-polynomials with closed forms, check |err| <= ~eps
  struct T { function<double(double)> f, F; double l, r; };
  for (int it = 0; it < 2000; it++) {
    double k = rndd(0.1, 5), l = rndd(-5, 5), r = l + rndd(0.01, 8);
    vector<T> ts = {
      {[&](double x) { return sin(k * x); }, [&](double x) { return -cos(k * x) / k; }, l, r},
      {[&](double x) { return exp(x / 2); }, [&](double x) { return 2 * exp(x / 2); }, l, r},
      {[&](double x) { return 1 / (1 + x * x); }, [&](double x) { return atan(x); }, l, r},
      {[&](double x) { return sqrt(x); }, [&](double x) { return 2.0 / 3 * pow(x, 1.5); }, 0, r + 5}, // sqrt singular derivative at 0
      {[&](double x) { return abs(x); }, [&](double x) { return x * abs(x) / 2; }, l, r},             // kink
    };
    for (int ti = 0; ti < (int)ts.size(); ti++) {
      auto &t = ts[ti];
      double want = t.F(t.r) - t.F(t.l);
      for (double eps : {1e-6, 1e-9}) {
        auto S = make_simpson(t.f);
        double got = S.eval(t.l, t.r, eps);
        // plain eval has no minimum depth: it can stop at the first level on oscillating / peaked
        // integrands (sin over several periods, Runge 1/(1+x^2)) -- that's what eval2 is for.
        if (ti == 0 || ti == 2) misses += abs(got - want) > 50 * eps;
        else assert(abs(got - want) <= 50 * eps);
        assert(abs(S.eval2(t.l, t.r, eps, 97) - want) <= 50 * eps);
        cases++;
      }
    }
  }
  // long double instantiation compiles and works
  { Simpson<function<long double(long double)>, long double> S{[](long double x) { return x * x; }};
    assert(fabsl(S.eval(0, 3, 1e-12L) - 9) < 1e-10); }
  printf("AdaptiveSimpson ok: %d integrals (cubics exact; sin/exp/atan/sqrt/|x| within 50*eps); plain eval missed %d sin/Runge cases, eval2 none\n", cases, misses);
}
