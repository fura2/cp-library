#pragma once

#include <concepts>
#include <utility>

#include "algebra/monoid_impl.hpp"
#include "template/constant.hpp"

template <typename T, auto Inf>
  requires requires {
    { Inf() } -> std::same_as<T>;
  } && requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
using ArgminMonoid =
    MonoidImpl<std::pair<T, int>,
               [](const auto& a, const auto& b) { return b < a ? b : a; },
               []() { return std::pair{Inf(), INF}; }>;

using IntArgminMonoid = ArgminMonoid<int, []() { return INF; }>;
using LintArgminMonoid = ArgminMonoid<long long, []() { return LINF; }>;
using DoubleArgminMonoid = ArgminMonoid<double, []() { return DINF; }>;
