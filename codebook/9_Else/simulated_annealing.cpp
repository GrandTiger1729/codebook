double factor = 100000;
const int base = 1e9; // remember to run ~ 10 times
FOR (it, 1, 1000000) {
  // ans: answer, nw: current value, rnd(): mt19937
  // rnd()
  if (exp(-(nw - ans) / factor) >=
    (double)(rnd() % base) / base)
    ans = nw;
  factor *= 0.99995;
}
