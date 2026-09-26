#pragma once

#include <optional>

#include "algebra/monoid_impl.hpp"

namespace first_monoid_impl {

template <typename T>
inline constexpr auto op =
    [](const std::optional<T>& a, const std::optional<T>& b) {
      return a.has_value() ? a : b;
    };

template <typename T>
inline constexpr auto id = []() -> std::optional<T> { return std::nullopt; };

}  // namespace first_monoid_impl

template <typename T>
using FirstMonoid = MonoidImpl<std::optional<T>,
                               first_monoid_impl::op<T>,
                               first_monoid_impl::id<T>>;
