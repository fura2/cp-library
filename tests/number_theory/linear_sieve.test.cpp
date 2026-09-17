#include "number_theory/linear_sieve.hpp"

#include <cassert>
#include <utility>
#include <vector>

int main() {
  const LinearSieve empty{0}, one{1}, sieve{60};
  assert(empty.primes().empty() && !empty.is_prime(0));
  assert(one.primes().empty() && !one.is_prime(1));
  assert((LinearSieve{10}.primes() == std::vector<int>{2, 3, 5, 7}));
  assert(sieve.is_prime(2) && sieve.is_prime(59));
  assert(!sieve.is_prime(49) && !sieve.is_prime(60));
  assert(sieve.prime_factorization(1).empty());
  assert((sieve.prime_factorization(60) ==
          std::vector<std::pair<int, int>>{{2, 2}, {3, 1}, {5, 1}}));
  assert((sieve.prime_factorization(49) ==
          std::vector<std::pair<int, int>>{{7, 2}}));
  assert((sieve.prime_factorization(59) ==
          std::vector<std::pair<int, int>>{{59, 1}}));
  assert(sieve.divisors(1) == std::vector<int>{1});
  assert((sieve.divisors(12) == std::vector<int>{1, 2, 3, 4, 6, 12}));
  assert((sieve.divisors(49) == std::vector<int>{1, 7, 49}));
}
