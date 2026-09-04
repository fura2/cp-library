#pragma once

#include <cassert>
#include <vector>

class EratosthenesSieve {
 public:
  explicit EratosthenesSieve(int n): n{n}, table((n + 1) / 2, true) {
    if (n >= 1) table[0] = false;
    for (int i = 3; i * i <= n; i += 2) {
      if (!table[i / 2]) continue;
      for (int j = i * i; j <= n; j += 2 * i) table[j / 2] = false;
    }
  }

  bool is_prime(int a) const {
    assert(0 <= a && a <= n);
    if (a % 2 == 0) return a == 2;
    return table[a / 2];
  }

  std::vector<int> primes() const {
    std::vector<int> ps;
    if (n >= 2) ps.emplace_back(2);
    for (int i = 0; i < (n + 1) / 2; ++i) {
      if (table[i]) ps.emplace_back(2 * i + 1);
    }
    return ps;
  }

 private:
  int n;
  std::vector<bool> table;
};
