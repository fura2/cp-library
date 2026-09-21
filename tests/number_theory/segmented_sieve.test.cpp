#include "number_theory/segmented_sieve.hpp"

#include <algorithm>
#include <cassert>
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
                         long long r,
                         const std::vector<long long>& expected_primes) {
    for (const auto& sieve: {SegmentedSieve{l, r}, SegmentedSieve{l, r, ps}}) {
      assert(sieve.primes() == expected_primes);
      for (long long a = l; a <= r; ++a) {
        assert(sieve.is_prime(a) == std::binary_search(expected_primes.begin(),
                                                       expected_primes.end(),
                                                       a));
      }
    }
  };

  check(0, 0, {});
  check(1, 1, {});
  check(2, 2, {2});
  check(0, 12, {2, 3, 5, 7, 11});
  check(1, 9, {2, 3, 5, 7});
  check(3, 9, {3, 5, 7});
  check(11, 13, {11, 13});
  check(47, 49, {47});
  check(49, 49, {});
  check(90, 96, {});
  check(4'294'967'311LL, 4'294'967'311LL, {4'294'967'311LL});
  check(65'537LL * 65'537, 65'537LL * 65'537 + 1, {});

  const SegmentedSieve no_base_primes{0, 3, {}};
  assert((no_base_primes.primes() == std::vector<long long>{2, 3}));
  assert(!no_base_primes.is_prime(0) && !no_base_primes.is_prime(1));
  assert(no_base_primes.is_prime(2) && no_base_primes.is_prime(3));
}
