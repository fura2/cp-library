#pragma once

#include <cassert>
#include <iterator>
#include <vector>

#include "number_theory/multiplicative_function.hpp"

inline const auto totient_prime_power = [](int p, int e) {
  int phi = p - 1;
  for (int i = 0; i < e - 1; ++i) {
    phi *= p;
  }
  return phi;
};

inline int totient(int a, const std::vector<int>& lpf) {
  assert(1 <= a && a < std::ssize(lpf));
  return multiplicative_function(a, totient_prime_power, lpf);
}

inline std::vector<int> totient_table(const std::vector<int>& lpf) {
  return multiplicative_function_table(totient_prime_power, lpf);
}
