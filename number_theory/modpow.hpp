#pragma once

#include <cassert>

inline long long modpow(long long a, long long k, int m) {
  assert(k >= 0);
  assert(m > 0);
  a %= m;
  if (a < 0) a += m;
  long long r = 1 % m;
  while (k > 0) {
    if (k & 1) r = r * a % m;
    a = a * a % m;
    k >>= 1;
  }
  return r;
}
