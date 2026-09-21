#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "number_theory/eratosthenes_sieve.hpp"
#include "util/isqrt.hpp"

class SegmentedSieve {
 public:
  SegmentedSieve(long long l, long long r): l{l}, r{r} {
    assert(0 <= l && l <= r);
    build(EratosthenesSieve(isqrt(r)).primes());
  }

  SegmentedSieve(long long l, long long r, const std::vector<int>& ps)
      : l{l}, r{r} {
    assert(0 <= l && l <= r);
    build(ps);
  }

  bool is_prime(long long a) const {
    assert(l <= a && a <= r);
    return table[a - l];
  }

  const std::vector<long long>& primes() const { return ps; }

 private:
  long long l, r;
  std::vector<bool> table;
  std::vector<long long> ps;

  void build(const std::vector<int>& ps) {
    table.assign(r - l + 1, true);
    for (int a = 0; a <= 1; ++a)
      if (l <= a && a <= r) table[a - l] = false;
    for (long long p: ps) {
      if (p * p > r) break;
      for (auto a = std::max((l + p - 1) / p, p) * p; a <= r; a += p) {
        table[a - l] = false;
      }
    }
    for (long long a = std::max(l, 2LL); a <= r; ++a) {
      if (table[a - l]) this->ps.emplace_back(a);
    }
  }
};
