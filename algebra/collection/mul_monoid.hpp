#pragma once

#include "algebra/monoid_impl.hpp"

namespace mul_monoid_detail {

template <typename T>
inline constexpr auto mul = [](const T& a, const T& b) -> T { return a * b; };

}  // namespace mul_monoid_detail

template <typename T, auto One>
  requires(
              requires {
                { One() } -> std::same_as<T>;
              } &&
              requires(const T& a, const T& b) {
                { a * b } -> std::same_as<T>;
              })
using MulMonoid = MonoidImpl<T, mul_monoid_detail::mul<T>, One>;

using IntMulMonoid = MulMonoid<int, []() { return 1; }>;
using LintMulMonoid = MulMonoid<long long, []() { return 1LL; }>;
using DoubleMulMonoid = MulMonoid<double, []() { return 1.0; }>;
