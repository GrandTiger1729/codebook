const int N = 1021;
struct CircleCover {
  int C;
  Cir c[N];
  bool g[N][N], overlap[N][N];
  // Area[i] : area covered by at least i circles
  double Area[ N ];
  void init(int _C){ C = _C;}
  struct Teve {
    pdd p; double ang; int add;
    bool operator<(const Teve &a)const
    {return ang < a.ang;}
  }eve[N * 2];
  // strict: x = 0, otherwise x = -1
  bool disjuct(Cir &a, Cir &b, int x)
  {return sign(abs(a.O - b.O) - a.R - b.R) > x;}
  bool contain(Cir &a, Cir &b, int x)
  {return sign(a.R - b.R - abs(a.O - b.O)) > x;}
  bool contain(int i, int j) {
    /* c[j] is non-strictly in c[i]. */
    int s = sign(c[i].R - c[j].R);
    return (s > 0 || (s == 0 && i < j)) &&
      contain(c[i], c[j], -1);
  }
  void solve(){
    fill_n(Area, C + 2, 0);
    FOR (i, 0, C - 1)
      FOR (j, 0, C - 1) overlap[i][j] = contain(i, j);
    FOR (i, 0, C - 1)
      FOR (j, 0, C - 1)
        g[i][j] = !(overlap[i][j] || overlap[j][i] ||
            disjuct(c[i], c[j], -1));
    FOR (i, 0, C - 1) {
      int E = 0, cnt = 1;
      pdd o = c[i].O; double R = c[i].R;
      FOR (j, 0, C - 1) if(j != i && overlap[j][i]) ++cnt;
      FOR (j, 0, C - 1)
        if(i != j && g[i][j]) {
          pdd aa, bb;
          CCinter(c[i], c[j], aa, bb);
          double A = atan2(aa.Y - o.Y, aa.X - o.X);
          double B = atan2(bb.Y - o.Y, bb.X - o.X);
          eve[E++] = {bb, B, 1}, eve[E++] = {aa, A, -1};
          if(B > A) ++cnt;
        }
      if(E == 0) Area[cnt] += acos(-1) * R * R;
      else{
        sort(eve, eve + E);
        eve[E] = eve[0];
        FOR (j, 0, E - 1) {
          cnt += eve[j].add;
          Area[cnt] += cross(eve[j].p, eve[j + 1].p) * .5;
          double th = eve[j + 1].ang - eve[j].ang;
          if (th < 0) th += 2 * acos(-1);
          Area[cnt] += (th - sin(th)) * R * R * .5;
        }
      }
    }
  }
};
