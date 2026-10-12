#pragma once

#include <cassert>

#include "number_theory/extended_gcd.hpp"

inline long long mod_inv(long long a, long long m) {
  assert(m > 0);
  auto [g, x, y] = extended_gcd(a, m);
  assert(g == 1);
  return x;
}
