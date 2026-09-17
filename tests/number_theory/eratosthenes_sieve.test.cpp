#include "number_theory/eratosthenes_sieve.hpp"

#include <cassert>
#include <vector>

int main() {
  const EratosthenesSieve empty{0}, one{1}, two{2}, sieve{30};
  assert(empty.primes().empty() && !empty.is_prime(0));
  assert(one.primes().empty() && !one.is_prime(1));
  assert(two.primes() == std::vector<int>{2});
  assert(
      (sieve.primes() == std::vector<int>{2, 3, 5, 7, 11, 13, 17, 19, 23, 29}));
  assert(sieve.is_prime(2) && sieve.is_prime(29));
  assert(!sieve.is_prime(0) && !sieve.is_prime(1));
  assert(!sieve.is_prime(9) && !sieve.is_prime(25) && !sieve.is_prime(30));
  assert(EratosthenesSieve{29}.is_prime(29));
}
