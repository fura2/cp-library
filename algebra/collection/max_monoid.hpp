#pragma once

#include <concepts>

#include "algebra/monoid_impl.hpp"
#include "template/constant.hpp"

template <typename T, auto NegInf>
  requires requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
using MaxMonoid =
    MonoidImpl<T, [](const T& a, const T& b) { return a < b ? b : a; }, NegInf>;

using IntMaxMonoid = MaxMonoid<int, []() { return -INF; }>;
using LintMaxMonoid = MaxMonoid<long long, []() { return -LINF; }>;
using DoubleMaxMonoid = MaxMonoid<double, []() { return -DINF; }>;
