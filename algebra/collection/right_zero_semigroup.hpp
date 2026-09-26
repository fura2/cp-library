#pragma once

#include "algebra/semigroup_impl.hpp"

namespace right_zero_semigroup_detail {

template <typename T>
inline constexpr auto op = [](const T&, const T& b) { return b; };

}  // namespace right_zero_semigroup_detail

template <typename T>
using RightZeroSemigroup = SemigroupImpl<T, right_zero_semigroup_detail::op<T>>;
