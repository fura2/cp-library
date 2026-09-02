#pragma once

#include <concepts>
#include <utility>

#include "algebra/monoid_impl.hpp"
#include "template/constant.hpp"

template <typename T, auto NegInf>
  requires requires {
    { NegInf() } -> std::same_as<T>;
  } && requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
using ArgmaxMonoid = MonoidImpl<std::pair<T, int>,
                                [](const auto& a, const auto& b) {
                                  return a.first < b.first ? b : a;
                                },
                                []() { return std::pair{NegInf(), -1}; }>;

using IntArgmaxMonoid = ArgmaxMonoid<int, []() { return -INF; }>;
using LintArgmaxMonoid = ArgmaxMonoid<long long, []() { return -LINF; }>;
using DoubleArgmaxMonoid = ArgmaxMonoid<double, []() { return -DINF; }>;
