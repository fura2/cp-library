#pragma once

#include <concepts>
#include <vector>

#include "algebra/collection/add_group.hpp"
#include "data_structure/fenwick_tree.hpp"
#include "util/compression.hpp"

template <std::integral T>
long long inversion_number(const std::vector<T>& a) {
  int n = a.size();
  Compression<T> cc(a);

  long long res = 0;
  FenwickTree<AddGroup<long long>> F(cc.size());
  for (int i = 0; i < n; ++i) {
    int r = cc.index(a[i]);
    res += i - F.fold(r + 1).unwrap();
    F.apply(r, 1);
  }
  return res;
}
