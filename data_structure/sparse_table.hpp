#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <vector>

#include "algebra/monoid.hpp"
#include "algebra/semigroup.hpp"

template <IdempotentSemigroup S>
class SparseTable {
 public:
  template <typename T = S>
    requires std::constructible_from<S, const T&>
  explicit SparseTable(const std::vector<T>& a): n(a.size()) {
    if (n == 0) return;
    int h = std::bit_width<unsigned int>(n);
    table.resize(h);
    table[0].reserve(n);
    for (const auto& x: a) {
      table[0].emplace_back(x);
    }
    for (int k = 0; k < h - 1; ++k) {
      int sz = n - (1 << (k + 1)) + 1;
      table[k + 1].reserve(sz);
      for (int i = 0; i < sz; ++i) {
        table[k + 1].emplace_back(table[k][i] * table[k][i + (1 << k)]);
      }
    }
  }

  int size() const { return n; }

  S fold(int l, int r) const {
    if constexpr (Monoid<S>) {
      assert(0 <= l && l <= r && r <= n);
      if (l == r) return S::identity();
    }
    else {
      assert(0 <= l && l < r && r <= n);
    }
    int k = std::bit_width<unsigned int>(r - l) - 1;
    return table[k][l] * table[k][r - (1 << k)];
  }

 private:
  int n;
  std::vector<std::vector<S>> table;
};
