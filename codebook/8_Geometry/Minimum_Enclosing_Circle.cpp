pdd Minimum_Enclosing_Circle(vector<pdd> dots, double &r) {
  pdd cent;
  static mt19937 rng(random_device{}());
  shuffle(ALL(dots), rng);
  cent = dots[0], r = 0;
  FOR (i, 1, SZ(dots) - 1)
    if (abs(dots[i] - cent) > r) {
      cent = dots[i], r = 0;
      FOR (j, 0, i - 1)
        if (abs(dots[j] - cent) > r) {
          cent = (dots[i] + dots[j]) / 2;
          r = abs(dots[i] - cent);
          FOR (k, 0, j - 1)
            if (abs(dots[k] - cent) > r) {
              cent =
                circenter(dots[i], dots[j], dots[k]);
              r = abs(dots[i] - cent);
            }
        }
    }
  return cent;
}
