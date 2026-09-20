#pragma once

#include <cassert>
#include <iterator>
#include <vector>

#include "number_theory/multiplicative_function.hpp"

inline const auto divisor_sum_prime_power = [](int p, int e) {
  long long sigma = 1;
  for (int i = 0; i < e; ++i) {
    sigma = 1 + p * sigma;
  }
  return sigma;
};

inline long long divisor_sum(int a, const std::vector<int>& lpf) {
  assert(1 <= a && a < std::ssize(lpf));
  return multiplicative_function(a, divisor_sum_prime_power, lpf);
}

inline std::vector<long long> divisor_sum_table(const std::vector<int>& lpf) {
  return multiplicative_function_table(divisor_sum_prime_power, lpf);
}
