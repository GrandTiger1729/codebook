#include "prelude.h"
#include "stress.h"
#include "1_Basic/readchar.cpp"
// feed files of many sizes (around and across the 1<<16 buffer) as stdin;
// readchar must return exactly the bytes, then keep returning after EOF
// only what's left in buf is unspecified, so we read exactly |file| bytes.
int main() {
  string path = "/tmp/readchar_stress_" + to_string(getpid());
  vector<size_t> sizes = {1, 2, 65535, 65536, 65537, 131072, 131073, 1000003};
  FOR (i, 1, 12) sizes.pb(rnd(1, 400000));
  // one process = one stdin, so all cases are concatenated into one file
  string all;
  for (size_t sz : sizes) FOR (j, 1, (int)sz) all += char(rnd(0, 255));
  { FILE *f = fopen(path.c_str(), "wb"); fwrite(all.data(), 1, all.size(), f); fclose(f); }
  assert(freopen(path.c_str(), "rb", stdin));
  for (size_t i = 0; i < all.size(); i++) assert(readchar() == all[i]);
  remove(path.c_str());
  printf("readchar: %zu bytes OK (%zu chunks incl. 2^16 +-1 boundaries)\n", all.size(), sizes.size());
}
