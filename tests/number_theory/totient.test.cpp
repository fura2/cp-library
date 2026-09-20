#include "number_theory/totient.hpp"

#include <cassert>
#include <concepts>
#include <numeric>
#include <vector>

#include "number_theory/linear_sieve.hpp"

int main() {
  const std::vector<int> one = {-1, -1};
  assert(totient(1, one) == 1);
  auto one_table = totient_table(one);
  assert(one_table.size() == 2 && one_table[1] == 1);

  const LinearSieve sieve{1000};
  const auto& lpf = sieve.least_prime_factors();
  assert(totient(1, lpf) == 1);
  assert(totient(2, lpf) == 1);
  assert(totient(4, lpf) == 2);
  assert(totient(12, lpf) == 4);
  assert(totient(30, lpf) == 8);
  assert(totient(32, lpf) == 16);
  assert(totient(49, lpf) == 42);
  assert(totient(59, lpf) == 58);
  assert(totient(1000, lpf) == 400);

  auto table = totient_table(lpf);
  static_assert(std::same_as<decltype(totient(1, lpf)), int>);
  static_assert(std::same_as<decltype(table), std::vector<int>>);
  assert(table.size() == 1001);
  for (int a = 1; a <= 1000; ++a) {
    int expected = 0;
    for (int b = 1; b <= a; ++b) {
      if (std::gcd(a, b) == 1) ++expected;
    }
    assert(totient(a, lpf) == expected);
    assert(table[a] == expected);
  }

  // Large prime powers can be checked without allocating a large LPF array.
  assert(totient_prime_power(2, 30) == 536870912);
  assert(totient_prime_power(2147483647, 1) == 2147483646);

  // The LPF array need not come from LinearSieve.
  const std::vector<int> supplied = {-1, -1, 2, 3, 2, 5, 2, 7, 2, 3, 2, 11, 2};
  assert(totient(12, supplied) == 4);
  auto supplied_table = totient_table(supplied);
  assert(supplied_table.size() == 13 && supplied_table[12] == 4);
}
