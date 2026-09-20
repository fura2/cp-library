#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>

inline std::vector<int> divisors(int a, const std::vector<int>& lpf) {
  assert(1 <= a && a < std::ssize(lpf));
  std::vector<int> ds = {1};
  while (a > 1) {
    int p = lpf[a], e = 0;
    while (lpf[a] == p) {
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
