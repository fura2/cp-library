#pragma once

#include "algebra/semigroup_impl.hpp"

namespace left_zero_semigroup_impl {

template <typename T>
inline constexpr auto op = [](const T& a, const T&) { return a; };

}  // namespace left_zero_semigroup_impl

template <typename T>
using LeftZeroSemigroup = SemigroupImpl<T, left_zero_semigroup_impl::op<T>>;
