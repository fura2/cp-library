#include "number_theory/num_divisors.hpp"

#include <cassert>
#include <concepts>
#include <vector>

#include "number_theory/linear_sieve.hpp"

int main() {
  const std::vector<int> one = {-1, -1};
  assert(num_divisors(1, one) == 1);
  auto one_table = num_divisors_table(one);
  assert(one_table.size() == 2 && one_table[1] == 1);

  const LinearSieve sieve{1000};
  const auto& lpf = sieve.least_prime_factors();
  assert(num_divisors(1, lpf) == 1);
  assert(num_divisors(2, lpf) == 2);
  assert(num_divisors(32, lpf) == 6);
  assert(num_divisors(49, lpf) == 3);
  assert(num_divisors(59, lpf) == 2);
  assert(num_divisors(60, lpf) == 12);
  assert(num_divisors(1000, lpf) == 16);

  auto table = num_divisors_table(lpf);
  static_assert(std::same_as<decltype(num_divisors(1, lpf)), int>);
  static_assert(std::same_as<decltype(table), std::vector<int>>);
  assert(table.size() == 1001);
  for (int a = 1; a <= 1000; ++a) {
    int expected = 0;
    for (int d = 1; d <= a; ++d) {
      if (a % d == 0) ++expected;
    }
    assert(num_divisors(a, lpf) == expected);
    assert(table[a] == expected);
  }

  // The LPF array need not come from LinearSieve.
  const std::vector<int> supplied = {-1, -1, 2, 3, 2, 5, 2, 7, 2, 3, 2, 11, 2};
  assert(num_divisors(12, supplied) == 6);
  auto supplied_table = num_divisors_table(supplied);
  assert(supplied_table.size() == 13 && supplied_table[12] == 6);
}
