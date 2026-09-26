#pragma once

#include <optional>

#include "algebra/monoid_impl.hpp"

namespace last_monoid_impl {

template <typename T>
inline constexpr auto op =
    [](const std::optional<T>& a, const std::optional<T>& b) {
      return b.has_value() ? b : a;
    };

template <typename T>
inline constexpr auto id = []() -> std::optional<T> { return std::nullopt; };

}  // namespace last_monoid_impl

template <typename T>
using LastMonoid = MonoidImpl<std::optional<T>,
                              last_monoid_impl::op<T>,
                              last_monoid_impl::id<T>>;
