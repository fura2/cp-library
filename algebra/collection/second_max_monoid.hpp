#pragma once

#include <concepts>
#include <utility>

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/monoid_impl.hpp"

namespace second_max_monoid_detail {

template <typename T>
  requires requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
inline constexpr auto op = [](const std::pair<T, T>& a,
                              const std::pair<T, T>& b) {
  return a.first < b.first ? std::pair{b.first, op_max<T>(a.first, b.second)}
                           : std::pair{a.first, op_max<T>(b.first, a.second)};
};

template <typename T>
  requires requires {
    { id_neg_inf<T>() } -> std::same_as<T>;
  }
inline constexpr auto id = []() -> std::pair<T, T> {
  return std::pair{id_neg_inf<T>(), id_neg_inf<T>()};
};

}  // namespace second_max_monoid_detail

template <typename T>
using SecondMaxMonoid = MonoidImpl<std::pair<T, T>,
                                   second_max_monoid_detail::op<T>,
                                   second_max_monoid_detail::id<T>>;
