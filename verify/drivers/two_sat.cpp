#include "../prelude.h"
const int MAXN = 4000006;           // >= 2 * (#vars + at_most_one extras) + 1
vector<int> adj[MAXN];
#include "../../codebook/2_Graph/SCC.cpp"
#include "../../codebook/2_Graph/2SAT.cpp"

int main() {
  Waimai;
  string s, cnf; int n, m;
  cin >> s >> cnf >> n >> m;   // "p cnf N M"
  TwoSat sat(n);
  // DIMACS literal v (1-based, negative = negated) -> 0-based, ~ for negation
  auto lit = [](int v) { return v > 0 ? v - 1 : ~(-v - 1); };
  for (int i = 0; i < m; i++) {
    int a, b, z; cin >> a >> b >> z;
    sat.either(lit(a), lit(b));
  }
  if (!sat.solve()) { cout << "s UNSATISFIABLE\n"; return 0; }
  cout << "s SATISFIABLE\nv";
  for (int i = 0; i < n; i++) cout << ' ' << (sat.ans[i] ? i + 1 : -(i + 1));
  cout << " 0\n";
}
