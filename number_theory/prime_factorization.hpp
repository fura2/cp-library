#pragma once

#include <cassert>
#include <iterator>
#include <utility>
#include <vector>

inline auto prime_factorization(int a, const std::vector<int>& lpf) {
  assert(1 <= a && a < std::ssize(lpf));
  std::vector<std::pair<int, int>> pf;
  while (a > 1) {
    int p = lpf[a], e = 0;
    while (lpf[a] == p) {
      a /= p;
      ++e;
    }
    pf.emplace_back(p, e);
  }
  return pf;
}
