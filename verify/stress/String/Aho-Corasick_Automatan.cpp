#include "prelude.h"
#include "stress.h"
const int N = 405, C = 3;
#include "5_String/Aho-Corasick_Automatan.cpp"
// cnt[] after build_fail() = how many patterns (with multiplicity) end at
// that node's string, so walking a text along to[] and summing cnt counts
// every (pattern, position) occurrence. Compared with the naive count;
// fail[] is compared with the longest proper suffix that is a trie node.
// reset() between cases checks that the object can be reused.
int main() {
  int cases = 0;
  FOR (it, 1, 100000) {
    int sig = rnd(1, C), k = rnd(1, 8), n = rnd(0, 40);
    auto gen = [&](int l) {
      string s;
      FOR (i, 1, l) s += char('a' + rnd(0, sig - 1));
      return s;
    };
    ac.reset();
    vector<string> pat(k);
    map<string, int> node;
    node[""] = 0;
    for (auto &p : pat) {
      p = gen(rnd(1, it % 10 ? 5 : 40));
      int v = ac.insert(p);
      assert(!node.count(p) || node[p] == -1 || node[p] == v);
      node[p] = v;
      FOR (l, 1, (int)p.size() - 1) node.emplace(p.substr(0, l), -1);
    }
    ac.build_fail();
    // fill in the ids of the inner prefixes by walking the trie
    for (auto &[s, v] : node) {
      int now = 0;
      for (char c : s) now = ac.ch[now][c - 'a'], assert(now);
      assert(v == -1 || v == now), v = now;
    }
    assert((int)node.size() == ac._id);
    for (auto &[s, v] : node) {
      int want = 0;
      FOR (l, 1, (int)s.size() - 1)
        if (node.count(s.substr(l))) { want = node[s.substr(l)]; break; }
      assert(ac.fail[v] == want);
    }
    string t = gen(n);
    ll want = 0, got = 0;
    for (auto &p : pat)
      for (int i = 0; i + p.size() <= t.size(); i++)
        want += t.compare(i, p.size(), p) == 0;
    int now = 0;
    for (char c : t) now = ac.to[now][c - 'a'], got += ac.cnt[now];
    assert(got == want);
    cases++;
  }
  printf("AC ok: %d random cases (sigma<=3, <=8 patterns, |t|<=40)\n", cases);
}
