#pragma once

#include <cassert>
#include <concepts>
#include <iterator>
#include <type_traits>
#include <vector>

template <typename F>
  requires std::invocable<F&, int, int>
auto multiplicative_function(int a,
                             F prime_power_value,
                             const std::vector<int>& lpf) {
  using T = std::invoke_result_t<F&, int, int>;

  assert(lpf.size() >= 2);
  assert(1 <= a && a < std::ssize(lpf));

  T res = 1;
  while (a > 1) {
    int p = lpf[a], e = 0;
    while (lpf[a] == p) {
      a /= p;
      ++e;
    }
    res *= prime_power_value(p, e);
  }
  return res;
}

template <typename F>
  requires std::invocable<F&, int, int>
auto multiplicative_function_table(F prime_power_value,
                                   const std::vector<int>& lpf) {
  using T = std::invoke_result_t<F&, int, int>;

  assert(lpf.size() >= 2);

  int n = lpf.size() - 1;
  std::vector<T> table(n + 1);
  table[1] = 1;
  for (int a = 2; a <= n; ++a) {
    int p = lpf[a], e = 0, b = a;
    while (lpf[b] == p) {
      b /= p;
      ++e;
    }
    table[a] = table[b] * prime_power_value(p, e);
  }
  return table;
}
