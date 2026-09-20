#include "number_theory/divisor_sum.hpp"

#include <cassert>
#include <concepts>
#include <vector>

#include "number_theory/linear_sieve.hpp"

int main() {
  const std::vector<int> one = {-1, -1};
  assert(divisor_sum(1, one) == 1);
  auto one_table = divisor_sum_table(one);
  assert(one_table.size() == 2 && one_table[1] == 1);

  const LinearSieve sieve{1000};
  const auto& lpf = sieve.least_prime_factors();
  assert(divisor_sum(1, lpf) == 1);
  assert(divisor_sum(2, lpf) == 3);
  assert(divisor_sum(32, lpf) == 63);
  assert(divisor_sum(49, lpf) == 57);
  assert(divisor_sum(59, lpf) == 60);
  assert(divisor_sum(60, lpf) == 168);
  assert(divisor_sum(1000, lpf) == 2340);

  auto table = divisor_sum_table(lpf);
  static_assert(std::same_as<decltype(divisor_sum(1, lpf)), long long>);
  static_assert(std::same_as<decltype(table), std::vector<long long>>);
  assert(table.size() == 1001);
  for (int a = 1; a <= 1000; ++a) {
    long long expected = 0;
    for (int d = 1; d <= a; ++d) {
      if (a % d == 0) expected += d;
    }
    assert(divisor_sum(a, lpf) == expected);
    assert(table[a] == expected);
  }

  // Large prime powers can be checked without allocating a large LPF array.
  assert(divisor_sum_prime_power(2, 30) == 2147483647LL);
  assert(divisor_sum_prime_power(2147483647, 1) == 2147483648LL);

  // The LPF array need not come from LinearSieve.
  const std::vector<int> supplied = {-1, -1, 2, 3, 2, 5, 2, 7, 2, 3, 2, 11, 2};
  assert(divisor_sum(12, supplied) == 28);
  auto supplied_table = divisor_sum_table(supplied);
  assert(supplied_table.size() == 13 && supplied_table[12] == 28);
}
