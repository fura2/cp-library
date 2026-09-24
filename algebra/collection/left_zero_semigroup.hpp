#pragma once

#include "algebra/semigroup_impl.hpp"

namespace left_zero_semigroup_impl {

inline constexpr auto op = [](const auto& a, const auto&) { return a; };

}  // namespace left_zero_semigroup_impl

template <typename T>
using LeftZeroSemigroup = SemigroupImpl<T, left_zero_semigroup_impl::op>;
