#pragma once

#include <cassert>
#include <utility>
#include <vector>

#include "number_theory/eratosthenes_sieve.hpp"
#include "util/isqrt.hpp"

inline auto segmented_prime_factorization(long long l,
                                          long long r,
                                          const std::vector<int>& ps) {
  assert(1 <= l && l <= r);

  std::vector<std::vector<std::pair<long long, int>>> pf(r - l + 1);
  for (int p: ps) {
    if (1LL * p * p > r) break;
    for (auto a = (l + p - 1) / p * p; a <= r; a += p) {
      pf[a - l].emplace_back(p, 0);
    }
  }
  for (auto a = l; a <= r; ++a) {
    auto b = a;
    for (auto& [p, e]: pf[a - l]) {
      do {
        ++e;
        b /= p;
      } while (b % p == 0);
    }
    if (b > 1) pf[a - l].emplace_back(b, 1);
  }
  return pf;
}

inline auto segmented_prime_factorization(long long l, long long r) {
  assert(1 <= l && l <= r);
  return segmented_prime_factorization(
      l, r, EratosthenesSieve(isqrt(r)).primes());
}
