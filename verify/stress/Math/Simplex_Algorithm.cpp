#include "prelude.h"
#include "stress.h"
#include "6_Math/Simplex_Algorithm.cpp"

// Brute force: vertices of {Ax<=b, x>=0} (m vars, n rows) by choosing m tight
// constraints out of the n+m, solving, and keeping the feasible points.
typedef vector<double> vd;
bool solveSys(vector<vd> M, vd r, vd &x) {
  int m = r.size();
  FOR (c, 0, m - 1) {
    int p = c;
    FOR (i, c, m - 1) if (fabs(M[i][c]) > fabs(M[p][c])) p = i;
    if (fabs(M[p][c]) < 1e-9) return false;
    swap(M[p], M[c]), swap(r[p], r[c]);
    FOR (i, 0, m - 1) if (i != c) {
      double f = M[i][c] / M[c][c];
      FOR (j, 0, m - 1) M[i][j] -= f * M[c][j];
      r[i] -= f * r[c];
    }
  }
  x.assign(m, 0);
  FOR (i, 0, m - 1) x[i] = r[i] / M[i][i];
  return true;
}
// returns {feasible?, best} over vertices of {Ax<=b, x>=0}
pair<bool, double> vertexMax(const vector<vd> &A, const vd &B, const vd &C) {
  int n = A.size(), m = C.size();
  vector<vd> rows(A);
  vd rhs(B);
  FOR (j, 0, m - 1) { vd e(m, 0); e[j] = -1; rows.pb(e), rhs.pb(0); }
  int T = n + m;
  bool feas = false;
  double best = -1e18;
  vector<int> pick(T, 0);
  fill(pick.end() - m, pick.end(), 1);
  do {
    vector<vd> M; vd r, x;
    FOR (i, 0, T - 1) if (pick[i]) M.pb(rows[i]), r.pb(rhs[i]);
    if (!solveSys(M, r, x)) continue;
    bool ok = true;
    FOR (i, 0, T - 1) {
      double s = 0;
      FOR (j, 0, m - 1) s += rows[i][j] * x[j];
      if (s > rhs[i] + 1e-7) ok = false;
    }
    if (!ok) continue;
    feas = true;
    double v = 0;
    FOR (j, 0, m - 1) v += C[j] * x[j];
    best = max(best, v);
  } while (next_permutation(pick.begin(), pick.end()));
  return {feas, best};
}
int main() {
  int cases = 0, cntInf = 0, cntUnb = 0, cntOpt = 0;
  FOR (it, 1, 12000) {
    int n = rnd(1, 4), m = rnd(1, 3), R = it % 2 ? 3 : 10;
    vector<vd> A(n, vd(m)); vd B(n), C(m);
    FOR (i, 0, n - 1) { FOR (j, 0, m - 1) A[i][j] = rnd(-R, R); B[i] = rnd(-R, R + R / 2); }
    FOR (j, 0, m - 1) C[j] = rnd(-R, R);
    // brute
    auto [feas, best] = vertexMax(A, B, C);
    int expect; // 0 infeasible, 1 unbounded, 2 optimum
    if (!feas) expect = 0;
    else {
      // recession direction d>=0, Ad<=0, sum d<=1 with c.d > 0 ?
      vector<vd> A2(A); vd B2(n, 0);
      A2.pb(vd(m, 1)), B2.pb(1);
      auto [f2, b2] = vertexMax(A2, B2, C);
      expect = b2 > 1e-7 ? 1 : 2;
    }
    if (expect == 2 && fabs(best + 1) < 1e-6) continue; // -1 collides with the error code
    FOR (i, 0, n - 1) { FOR (j, 0, m - 1) a[i][j] = A[i][j]; b[i] = B[i]; }
    FOR (j, 0, m - 1) c[j] = C[j];
    double got = simplex(n, m);
    if (expect == 2) {
      assert(fabs(got - best) < 1e-6);
      // x[] must be a feasible optimal point
      double v = 0;
      FOR (j, 0, m - 1) assert(x[j] > -1e-7), v += C[j] * x[j];
      FOR (i, 0, n - 1) {
        double s = 0;
        FOR (j, 0, m - 1) s += A[i][j] * x[j];
        assert(s <= B[i] + 1e-6);
      }
      assert(fabs(v - best) < 1e-6);
      cntOpt++;
    } else {
      assert(got == -1);
      (expect ? cntUnb : cntInf)++;
    }
    cases++;
  }
  // larger: strong duality. primal max cx, Ax<=b, x>=0 (b>=0 so feasible, c<=... random)
  // dual   min by, A^T y>=c, y>=0  ==  -max (-b)y, (-A^T)y <= -c
  int dual = 0;
  FOR (it, 1, 300) {
    int n = rnd(1, 25), m = rnd(1, 25);
    vector<vd> A(n, vd(m)); vd B(n), C(m);
    FOR (i, 0, n - 1) { FOR (j, 0, m - 1) A[i][j] = rnd(-3, 9); B[i] = rnd(0, 50); }
    FOR (j, 0, m - 1) C[j] = rnd(-5, 10);
    FOR (i, 0, n - 1) { FOR (j, 0, m - 1) a[i][j] = A[i][j]; b[i] = B[i]; }
    FOR (j, 0, m - 1) c[j] = C[j];
    double p = simplex(n, m);
    FOR (j, 0, m - 1) { FOR (i, 0, n - 1) a[j][i] = -A[i][j]; b[j] = -C[j]; }
    FOR (i, 0, n - 1) c[i] = -B[i];
    double d = simplex(m, n);
    if (fabs(p - 1) < 1e-9) continue; // dual optimum -1 collides with the error code
    if (p == -1 || d == -1) { assert(p == -1 && d == -1); continue; } // unbounded <-> dual infeasible
    assert(fabs(p + d) < 1e-6 * max(1.0, fabs(p))), dual++;
  }
  printf("Simplex: %d cases OK (%d optimal, %d unbounded, %d infeasible; %d duality)\n", cases, cntOpt, cntUnb, cntInf, dual);
}
