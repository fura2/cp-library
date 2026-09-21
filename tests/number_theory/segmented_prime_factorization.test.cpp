#include "number_theory/segmented_prime_factorization.hpp"

#include <cassert>
#include <iterator>
#include <utility>
#include <vector>

#include "number_theory/eratosthenes_sieve.hpp"

int main() {
  using Factorization = std::vector<std::pair<long long, int>>;
  const auto ps = EratosthenesSieve{100'000}.primes();
  const auto check = [&](long long l,
                         const std::vector<Factorization>& expected) {
    const long long r = l + std::ssize(expected) - 1;
    assert(segmented_prime_factorization(l, r) == expected);
    assert(segmented_prime_factorization(l, r, ps) == expected);
  };

  check(1, {{}});
  check(2, {{{2, 1}}});
  check(1,
        {{},
         {{2, 1}},
         {{3, 1}},
         {{2, 2}},
         {{5, 1}},
         {{2, 1}, {3, 1}},
         {{7, 1}},
         {{2, 3}},
         {{3, 2}},
         {{2, 1}, {5, 1}},
         {{11, 1}},
         {{2, 2}, {3, 1}}});
  check(47, {{{47, 1}}, {{2, 4}, {3, 1}}, {{7, 2}}});
  check(49, {{{7, 2}}});
  check(60, {{{2, 2}, {3, 1}, {5, 1}}});
  check(72, {{{2, 3}, {3, 2}}});
  check(90, {{{2, 1}, {3, 2}, {5, 1}}, {{7, 1}, {13, 1}}});
  check(1LL << 32, {{{2, 32}}});
  check(65'537LL * 65'537, {{{65'537, 2}}});
  check(4'294'967'311LL, {{{4'294'967'311LL, 1}}});
  check(2 * 4'294'967'311LL, {{{2, 1}, {4'294'967'311LL, 1}}});

  assert((segmented_prime_factorization(1, 3, {}) ==
          std::vector<Factorization>{{}, {{2, 1}}, {{3, 1}}}));
  assert((segmented_prime_factorization(6, 8, {2}) ==
          std::vector<Factorization>{{{2, 1}, {3, 1}}, {{7, 1}}, {{2, 3}}}));
}
