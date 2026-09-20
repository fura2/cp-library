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
    return lpf[a - l] == a;
  }

  const std::vector<long long>& primes() const { return ps; }

  long long least_prime_factor(long long a) const {
    assert(std::max(l, 2LL) <= a && a <= r);
    return lpf[a - l];
  }

  const std::vector<long long>& least_prime_factors() const { return lpf; }

 private:
  long long l, r;
  std::vector<long long> lpf;
  std::vector<long long> ps;

  void build(const std::vector<int>& ps) {
    lpf.assign(r - l + 1, -1);
    for (int p: ps) {
      if (1LL * p * p > r) break;
      for (auto a = std::max<long long>((l + p - 1) / p, p) * p; a <= r;
           a += p) {
        if (lpf[a - l] == -1) lpf[a - l] = p;
      }
    }
    for (long long a = std::max(l, 2LL); a <= r; ++a) {
      if (lpf[a - l] == -1) {
        lpf[a - l] = a;
        this->ps.emplace_back(a);
      }
    }
  }
};
