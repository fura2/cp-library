#include "number_theory/divisors.hpp"

#include <cassert>
#include <vector>

#include "number_theory/linear_sieve.hpp"

int main() {
  assert(divisors(1, std::vector<int>{-1, -1}) == std::vector<int>{1});

  const LinearSieve sieve{60};
  const auto& lpf = sieve.least_prime_factors();
  assert(divisors(1, lpf) == std::vector<int>{1});
  assert((divisors(2, lpf) == std::vector<int>{1, 2}));
  assert((divisors(12, lpf) == std::vector<int>{1, 2, 3, 4, 6, 12}));
  assert((divisors(32, lpf) == std::vector<int>{1, 2, 4, 8, 16, 32}));
  assert((divisors(49, lpf) == std::vector<int>{1, 7, 49}));
  assert((divisors(59, lpf) == std::vector<int>{1, 59}));
  assert((divisors(60, lpf) ==
          std::vector<int>{1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60}));

  // The LPF array need not come from LinearSieve.
  const std::vector<int> supplied = {-1, -1, 2, 3, 2, 5, 2, 7, 2, 3, 2, 11, 2};
  assert((divisors(12, supplied) == std::vector<int>{1, 2, 3, 4, 6, 12}));
}
