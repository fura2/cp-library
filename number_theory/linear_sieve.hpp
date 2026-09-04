#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

class LinearSieve {
 public:
  explicit LinearSieve(int n): n{n}, lpf(n + 1) {
    for (int i = 2; i <= n; i++) {
      if (lpf[i] == 0) {
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

  const std::vector<int>& primes() const { return ps; }

  bool is_prime(int a) const {
    assert(0 <= a && a <= n);
    return a >= 2 && lpf[a] == a;
  }

  std::vector<int> divisors(int a) const {
    assert(1 <= a && a <= n);
    std::vector<int> ds = {1};
    while (a > 1) {
      int p = lpf[a], e = 0;
      while (a >= 2 && lpf[a] == p) {
        a /= p;
        ++e;
      }
      int m = ds.size();
      for (int i = 0; i < e; ++i) {
        for (int j = 0; j < m; ++j) {
          ds.emplace_back(ds[i * m + j] * p);
        }
      }
    }
    std::ranges::sort(ds);
    return ds;
  }

 private:
  int n;
  std::vector<int> lpf, ps;
};
