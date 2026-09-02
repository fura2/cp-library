#pragma once

#include "algebra/semiring_impl.hpp"
#include "template/constant.hpp"

template <typename T, auto NegInf, auto Zero>
  requires requires {
    { NegInf() } -> std::same_as<T>;
  } && requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
    { a + b } -> std::same_as<T>;
  }
using MaxPlusSemiring =
    SemiringImpl<T,
                 [](const T& a, const T& b) { return a < b ? b : a; },
                 [](const T& a, const T& b) {
                   if (a == NegInf() || b == NegInf()) return NegInf();
                   return a + b;
                 },
                 NegInf,
                 Zero>;

using IntMaxPlusSemiring =
    MaxPlusSemiring<int, []() { return -INF; }, []() { return 0; }>;
using LintMaxPlusSemiring =
    MaxPlusSemiring<long long, []() { return -LINF; }, []() { return 0LL; }>;
using DoubleMaxPlusSemiring =
    MaxPlusSemiring<double, []() { return -DINF; }, []() { return 0.0; }>;
