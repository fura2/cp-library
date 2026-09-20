#pragma once

#include <cassert>
#include <vector>

class LinearSieve {
 public:
  explicit LinearSieve(int n): n{n}, lpf(n + 1, -1) {
    for (int i = 2; i <= n; i++) {
      if (lpf[i] == -1) {
        lpf[i] = i;
        ps.emplace_back(i);
      }
      for (int p: ps) {
        if (1LL * i * p > n) break;
        lpf[i * p] = p;
        if (p == lpf[i]) break;
      }
    }
  }

  bool is_prime(int a) const {
    assert(0 <= a && a <= n);
    return lpf[a] == a;
  }

  const std::vector<int>& primes() const { return ps; }

  int least_prime_factor(int a) const {
    assert(2 <= a && a <= n);
    return lpf[a];
  }

  const std::vector<int>& least_prime_factors() const { return lpf; }

 private:
  int n;
  std::vector<int> lpf, ps;
};
