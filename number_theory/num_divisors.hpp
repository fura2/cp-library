#pragma once

#include <vector>

#include "number_theory/multiplicative_function.hpp"

inline const auto num_divisors_prime_power = [](int, int e) { return e + 1; };

inline int num_divisors(int a, const std::vector<int>& lpf) {
  return multiplicative_function(a, num_divisors_prime_power, lpf);
}

inline std::vector<int> num_divisors_table(const std::vector<int>& lpf) {
  return multiplicative_function_table(num_divisors_prime_power, lpf);
}
