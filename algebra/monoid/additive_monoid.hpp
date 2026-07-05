#pragma once

template <typename T>
struct AdditiveMonoid {
  using value_type = T;
  static constexpr value_type op(const value_type& a, const value_type& b) {
    return a + b;
  }
  static constexpr value_type id() { return {}; }
};
