#pragma once

#include <concepts>
#include <utility>

#include "algebra/collection/id.hpp"
#include "algebra/monoid_impl.hpp"

namespace argmax_monoid_detail {

template <typename T>
  requires requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
inline constexpr auto op =
    [](const std::pair<T, int>& a, const std::pair<T, int>& b) {
      return a.first < b.first ? b : a;
    };

template <typename T>
  requires requires {
    { id_neg_inf<T>() } -> std::same_as<T>;
  }
inline constexpr auto id =
    []() -> std::pair<T, int> { return std::pair{id_neg_inf<T>(), -1}; };

}  // namespace argmax_monoid_detail

template <typename T>
using ArgmaxMonoid = MonoidImpl<std::pair<T, int>,
                                argmax_monoid_detail::op<T>,
                                argmax_monoid_detail::id<T>>;
