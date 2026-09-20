#include "number_theory/prime_factorization.hpp"

#include <cassert>
#include <utility>
#include <vector>

#include "number_theory/linear_sieve.hpp"

int main() {
  assert(prime_factorization(1, std::vector<int>{-1, -1}).empty());

  const LinearSieve sieve{60};
  const auto& lpf = sieve.least_prime_factors();
  assert(prime_factorization(1, lpf).empty());
  assert((prime_factorization(2, lpf) ==
          std::vector<std::pair<int, int>>{{2, 1}}));
  assert((prime_factorization(32, lpf) ==
          std::vector<std::pair<int, int>>{{2, 5}}));
  assert((prime_factorization(49, lpf) ==
          std::vector<std::pair<int, int>>{{7, 2}}));
  assert((prime_factorization(59, lpf) ==
          std::vector<std::pair<int, int>>{{59, 1}}));
  assert((prime_factorization(60, lpf) ==
          std::vector<std::pair<int, int>>{{2, 2}, {3, 1}, {5, 1}}));

  // The LPF array need not come from LinearSieve.
  const std::vector<int> supplied = {-1, -1, 2, 3, 2, 5, 2, 7, 2, 3, 2, 11, 2};
  assert((prime_factorization(12, supplied) ==
          std::vector<std::pair<int, int>>{{2, 2}, {3, 1}}));
}
