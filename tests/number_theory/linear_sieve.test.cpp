#include "number_theory/linear_sieve.hpp"

#include <cassert>
#include <vector>

int main() {
  const LinearSieve empty{0}, one{1}, two{2}, sieve{60};
  assert(empty.primes().empty() && !empty.is_prime(0));
  assert(empty.least_prime_factors() == std::vector<int>{-1});
  assert(one.primes().empty() && !one.is_prime(0) && !one.is_prime(1));
  assert((one.least_prime_factors() == std::vector<int>{-1, -1}));
  assert(two.primes() == std::vector<int>{2} && two.is_prime(2));
  assert((two.least_prime_factors() == std::vector<int>{-1, -1, 2}));
  assert((LinearSieve{10}.primes() == std::vector<int>{2, 3, 5, 7}));
  assert((LinearSieve{12}.least_prime_factors() ==
          std::vector<int>{-1, -1, 2, 3, 2, 5, 2, 7, 2, 3, 2, 11, 2}));
  assert(!sieve.is_prime(0) && !sieve.is_prime(1));
  assert(sieve.is_prime(2) && sieve.is_prime(59));
  assert(!sieve.is_prime(49) && !sieve.is_prime(60));
  const auto& lpf = sieve.least_prime_factors();
  assert(lpf.size() == 61);
  assert(lpf[49] == 7 && lpf[59] == 59 && lpf[60] == 2);
}
