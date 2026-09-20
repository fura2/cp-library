#include "number_theory/segmented_sieve.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

#include "number_theory/eratosthenes_sieve.hpp"

static_assert(
    std::is_same_v<decltype(std::declval<const SegmentedSieve&>().primes()),
                   const std::vector<long long>&>);

int main() {
  const auto ps = EratosthenesSieve{100'000}.primes();
  const auto check = [&](long long l,
                         const std::vector<long long>& expected_lpf,
                         const std::vector<long long>& expected_primes) {
    const long long r = l + std::ssize(expected_lpf) - 1;
    for (const auto& sieve: {SegmentedSieve{l, r}, SegmentedSieve{l, r, ps}}) {
      assert(sieve.least_prime_factors() == expected_lpf);
      assert(sieve.primes() == expected_primes);
      for (long long a = l; a <= r; ++a) {
        assert(sieve.is_prime(a) == std::binary_search(expected_primes.begin(),
                                                       expected_primes.end(),
                                                       a));
        if (a >= 2) assert(sieve.least_prime_factor(a) == expected_lpf[a - l]);
      }
    }
  };

  check(0, {-1}, {});
  check(1, {-1}, {});
  check(2, {2}, {2});
  check(0, {-1, -1, 2, 3, 2, 5, 2, 7, 2, 3, 2, 11, 2}, {2, 3, 5, 7, 11});
  check(1, {-1, 2, 3, 2, 5, 2, 7, 2, 3}, {2, 3, 5, 7});
  check(3, {3, 2, 5, 2, 7, 2, 3}, {3, 5, 7});
  check(11, {11, 2, 13}, {11, 13});
  check(47, {47, 2, 7}, {47});
  check(49, {7}, {});
  check(90, {2, 7, 2, 3, 2, 5, 2}, {});
  check(4'294'967'311LL, {4'294'967'311LL}, {4'294'967'311LL});
  check(65'537LL * 65'537, {65'537, 2}, {});

  const SegmentedSieve no_base_primes{0, 3, {}};
  assert((no_base_primes.primes() == std::vector<long long>{2, 3}));
  assert((no_base_primes.least_prime_factors() ==
          std::vector<long long>{-1, -1, 2, 3}));
}
