#pragma once

#include <concepts>
#include <utility>

#include "algebra/collection/id.hpp"
#include "algebra/monoid_impl.hpp"

namespace argmin_monoid_detail {

template <typename T>
  requires requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
inline constexpr auto op =
    [](const std::pair<T, int>& a, const std::pair<T, int>& b) {
      return b.first < a.first ? b : a;
    };

template <typename T>
  requires requires {
    { id_inf<T>() } -> std::same_as<T>;
  }
inline constexpr auto id =
    []() -> std::pair<T, int> { return std::pair{id_inf<T>(), -1}; };

}  // namespace argmin_monoid_detail

template <typename T>
using ArgminMonoid = MonoidImpl<std::pair<T, int>,
                                argmin_monoid_detail::op<T>,
                                argmin_monoid_detail::id<T>>;
