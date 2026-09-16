#define SZ(a) ((int)a.size())
// a_i = \sum_{j=1}^{k} c_j a_(i-j); returns c 1-based,
// c[0] unused, k = SZ(c) - 1. Feeds LinearRecursion.
template <typename T>
vector<T> BerlekampMassey(const vector<T> &output) {
  vector<T> d(SZ(output) + 1), me, he;
  for (int f = 0, i = 1; i <= SZ(output); i++) {
    FOR (j, 0, SZ(me) - 1)
      d[i] += output[i - j - 2] * me[j];
    if ((d[i] -= output[i - 1]) == 0) continue;
    if (me.empty()) {
      me.resize(f = i);
      continue;
    }
    vector<T> o(i - f - 1);
    T k = -d[i] / d[f]; o.pb(-k);
    for (T x : he) o.pb(x * k);
    o.resize(max(SZ(o), SZ(me)));
    FOR (j, 0, SZ(me) - 1) o[j] += me[j];
    if (i - f + SZ(he) >= SZ(me)) he = me, f = i;
    me = o;
  }
  me.insert(me.begin(), T()); // me[j] was c_(j+1)
  return me;
}
