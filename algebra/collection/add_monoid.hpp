#pragma once

#include <concepts>

#include "algebra/monoid_impl.hpp"

namespace add_monoid_detail {

template <typename T>
inline constexpr auto add = [](const T& a, const T& b) -> T { return a + b; };

}  // namespace add_monoid_detail

template <typename T, auto Zero>
  requires(
              requires {
                { Zero() } -> std::same_as<T>;
              } &&
              requires(const T& a, const T& b) {
                { a + b } -> std::same_as<T>;
              })
using AddMonoid = MonoidImpl<T, add_monoid_detail::add<T>, Zero>;
