#include "number_theory/moebius.hpp"

#include <cassert>
#include <concepts>
#include <vector>

#include "number_theory/linear_sieve.hpp"

int main() {
  const std::vector<int> one = {-1, -1};
  assert(moebius(1, one) == 1);
  auto one_table = moebius_table(one);
  assert(one_table.size() == 2 && one_table[1] == 1);

  const LinearSieve sieve{1000};
  const auto& lpf = sieve.least_prime_factors();
  assert(moebius(1, lpf) == 1);
  assert(moebius(2, lpf) == -1);
  assert(moebius(6, lpf) == 1);
  assert(moebius(30, lpf) == -1);
  assert(moebius(210, lpf) == 1);
  assert(moebius(4, lpf) == 0);
  assert(moebius(12, lpf) == 0);
  assert(moebius(49, lpf) == 0);
  assert(moebius(997, lpf) == -1);
  assert(moebius(1000, lpf) == 0);

  auto table = moebius_table(lpf);
  static_assert(std::same_as<decltype(moebius(1, lpf)), int>);
  static_assert(std::same_as<decltype(table), std::vector<int>>);
  assert(table.size() == 1001);

  // mu(1) = 1 and sum_{d|a} mu(d) = 0 for a > 1.
  // Compute the expected values independently of prime factorization.
  std::vector<int> expected(1001);
  expected[1] = 1;
  for (int a = 1; a <= 1000; ++a) {
    for (int d = 1; d < a; ++d) {
      if (a % d == 0) expected[a] -= expected[d];
    }
    assert(moebius(a, lpf) == expected[a]);
    assert(table[a] == expected[a]);
  }

  // The LPF array need not come from LinearSieve.
  const std::vector<int> supplied = {-1, -1, 2, 3, 2, 5, 2, 7, 2, 3, 2, 11, 2};
  assert(moebius(6, supplied) == 1 && moebius(12, supplied) == 0);
  auto supplied_table = moebius_table(supplied);
  assert(supplied_table.size() == 13);
  assert(supplied_table[6] == 1 && supplied_table[12] == 0);
}
