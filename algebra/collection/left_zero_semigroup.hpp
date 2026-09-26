#pragma once

#include "algebra/semigroup_impl.hpp"

namespace left_zero_semigroup_detail {

template <typename T>
inline constexpr auto op = [](const T& a, const T&) { return a; };

}  // namespace left_zero_semigroup_detail

template <typename T>
using LeftZeroSemigroup = SemigroupImpl<T, left_zero_semigroup_detail::op<T>>;
