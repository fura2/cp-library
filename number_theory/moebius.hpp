#pragma once

#include <cassert>
#include <iterator>
#include <vector>

#include "number_theory/multiplicative_function.hpp"

inline const auto moebius_prime_power = [](int, int e) {
  return e == 1 ? -1 : 0;
};

inline int moebius(int a, const std::vector<int>& lpf) {
  assert(1 <= a && a < std::ssize(lpf));
  return multiplicative_function(a, moebius_prime_power, lpf);
}

inline std::vector<int> moebius_table(const std::vector<int>& lpf) {
  return multiplicative_function_table(moebius_prime_power, lpf);
}
