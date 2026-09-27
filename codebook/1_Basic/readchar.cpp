inline char readchar() {
  const int B = 1 << 16;
  static char buf[B], *p = buf, *end = buf;
  if (p == end)
    end = buf + fread_unlocked(p = buf, 1, B, stdin);
  return *p++;
}
