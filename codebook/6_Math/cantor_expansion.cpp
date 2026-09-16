#define MAXN 11
int factorial[MAXN];
inline void init(){
  factorial[0]=1;
  FOR (i, 1, MAXN) {
    factorial[i]=factorial[i-1]*i;
  }
}
inline int encode(const std::vector<int> &s){
  int n=s.size(),res=0;
  FOR (i, 0, n - 1) {
    int t=0;
    FOR (j, i + 1, n - 1) {
      if(s[j]<s[i])++t;
    }
    res+=t*factorial[n-i-1];
  }
  return res;
}
inline std::vector<int> decode(int a,int n){
  std::vector<int> res;
  std::vector<bool> vis(n,0);
  for(int i=n-1;i>=0;--i){
    int t=a/factorial[i],j;
    for (j = 0; j < n; j++) {
      if(!vis[j]){
        if(t==0)break;
        --t;
      }
    }
    res.pb(j);
    vis[j]=1;
    a%=factorial[i];
  }
  return res;
}
