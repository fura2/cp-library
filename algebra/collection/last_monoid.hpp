#pragma once

#include <optional>

#include "algebra/monoid_impl.hpp"

namespace last_monoid_impl {

inline constexpr auto op = [](const auto& a, const auto& b) {
  return b.has_value() ? b : a;
};

template <typename T>
inline constexpr auto id = []() -> std::optional<T> { return std::nullopt; };

}  // namespace last_monoid_impl

template <typename T>
using LastMonoid =
    MonoidImpl<std::optional<T>, last_monoid_impl::op, last_monoid_impl::id<T>>;
