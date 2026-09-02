#pragma once

#include <concepts>

#include "algebra/monoid_impl.hpp"
#include "template/constant.hpp"

template <typename T, auto Inf>
  requires requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
using MinMonoid =
    MonoidImpl<T, [](const T& a, const T& b) { return b < a ? b : a; }, Inf>;

using IntMinMonoid = MinMonoid<int, []() { return INF; }>;
using LintMinMonoid = MinMonoid<long long, []() { return LINF; }>;
using DoubleMinMonoid = MinMonoid<double, []() { return DINF; }>;
