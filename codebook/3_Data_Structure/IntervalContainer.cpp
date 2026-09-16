typedef pair<int, int> pii;
/* Add and remove intervals from a set of disjoint intervals.
 * Will merge the added interval with any overlapping intervals in the set when adding.
 * Intervals are [inclusive, exclusive). */
set<pii>::iterator addInterval(set<pii>& is, int L, int R) {
  if (L == R) return is.end();
  auto it = is.lower_bound({L, R}), before = it;
  while (it != is.end() && it->F <= R) {
    R = max(R, it->S);
    before = it = is.erase(it);
  }
  if (it != is.begin() && (--it)->S >= L) {
    L = min(L, it->F);
    R = max(R, it->S);
    is.erase(it);
  }
  return is.insert(before, pii(L, R));
}
void removeInterval(set<pii>& is, int L, int R) {
  if (L == R) return;
  auto it = addInterval(is, L, R);
  auto r2 = it->S;
  if (it->F == L) is.erase(it);
  else (int &)it->S = L;
  if (R != r2) is.emplace(R, r2);
}
