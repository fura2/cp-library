#pragma once

#include "algebra/semigroup_impl.hpp"

namespace right_zero_semigroup_impl {

inline constexpr auto op = [](const auto&, const auto& b) { return b; };

}  // namespace right_zero_semigroup_impl

template <typename T>
using RightZeroSemigroup = SemigroupImpl<T, right_zero_semigroup_impl::op>;
