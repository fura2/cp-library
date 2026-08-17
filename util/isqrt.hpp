#pragma once

#include <cassert>
#include <cmath>

inline long long isqrt(long long n) {
  assert(n >= 0);
  long long r = std::sqrt(n);
  if (r * r > n) --r;
  return r;
}
