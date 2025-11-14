#include <sys/personality.h>
#include <bits/stdc++.h>
int main(int argc, char** argv){
  personality(ADDR_NO_RANDOMIZE);
  execv(argv[1],argv+1);
}
