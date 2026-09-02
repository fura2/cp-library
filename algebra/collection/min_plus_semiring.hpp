#pragma once

#include "algebra/semiring_impl.hpp"
#include "template/constant.hpp"

template <typename T, auto Inf, auto Zero>
  requires requires {
    { Inf() } -> std::same_as<T>;
  } && requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
    { a + b } -> std::same_as<T>;
  }
using MinPlusSemiring =
    SemiringImpl<T,
                 [](const T& a, const T& b) { return b < a ? b : a; },
                 [](const T& a, const T& b) {
                   if (a == Inf() || b == Inf()) return Inf();
                   return a + b;
                 },
                 Inf,
                 Zero>;

using IntMinPlusSemiring =
    MinPlusSemiring<int, []() { return INF; }, []() { return 0; }>;
using LintMinPlusSemiring =
    MinPlusSemiring<long long, []() { return LINF; }, []() { return 0LL; }>;
using DoubleMinPlusSemiring =
    MinPlusSemiring<double, []() { return DINF; }, []() { return 0.0; }>;
