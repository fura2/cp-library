#pragma once

#include <algorithm>

#include "template/constant.hpp"

struct MinPlusSemiring {
  using value_type = long long;
  static constexpr value_type add(value_type a, value_type b) {
    return std::min(a, b);
  }
  static constexpr value_type mul(value_type a, value_type b) {
    if (a == zero() || b == zero()) return zero();
    return a + b;
  }
  static constexpr value_type zero() { return LINF; }
  static constexpr value_type one() { return 0LL; }
};
