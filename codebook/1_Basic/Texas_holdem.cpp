char suit[4]={'C','D','H','S'},ranks[13]={'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
int rk[256];
/*
   FOR (i, 0, 12)
   rk[ranks[i]]=i;
   FOR (i, 0, 3)
   rk[suit[i]]=i;
   */
struct cards{
  vector<pair<int, int>> v;
  int suit_count[4],hands;
  void reset(){v.clear(),FILL(suit_count,0),hands=-1;}
  void insert(char a,char b){//suit,rank
    ++suit_count[rk[a]];
    int flag=0;
    for(auto &i:v)
      if (i.S == rk[b]) {
        ++i.F, flag = 1;
        break;
      }
    if (!flag) v.pb(pair<int, int>(1, rk[b]));
  }
  void insert(string s){insert(s[0],s[1]);}
  void ready(){
    int Straight=0,Flush=(*max_element(suit_count,suit_count+4)==5);
    sort(v.begin(), v.end(),
      [](ii a, ii b) { return a > b; });
    if ((int)v.size() == 5 && v[0].S == v[1].S + 1 &&
      v[1].S == v[2].S + 1 && v[2].S == v[3].S + 1 &&
      v[3].S == v[4].S + 1)
      Straight=1;
    else if ((int)v.size() == 5 && v[0].S == 12 &&
      v[1].S == 3 && v[2].S == 2 && v[3].S == 1 &&
      v[4].S == 0)
      v[0].S = 3, v[1].S = 2, v[2].S = 1, v[1].S = 0,
      v[0].S = -1, Straight = 1;
    if(Straight&&Flush) hands=1;
    else if (v[0].F == 4) hands = 2;
    else if (v[0].F == 3 && v[1].F == 2) hands = 3;
    else if(Flush) hands=4;
    else if(Straight) hands=5;
    else if (v[0].F == 3) hands = 6;
    else if (v[0].F == 2 && v[1].F == 2) hands = 7;
    else if (v[0].F == 2) hands = 8;
    else hands=9;
  }
  bool operator>(const cards &a)const{
    if(hands==a.hands) return v>a.v;
    return hands<a.hands;
  }
};
