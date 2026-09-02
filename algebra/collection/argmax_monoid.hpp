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
                                  if (a.first < b.first) return b;
                                  if (b.first < a.first) return a;
                                  return b.second < a.second ? b : a;
                                },
                                []() { return std::pair{NegInf(), INF}; }>;

using IntArgmaxMonoid = ArgmaxMonoid<int, []() { return -INF; }>;
using LintArgmaxMonoid = ArgmaxMonoid<long long, []() { return -LINF; }>;
using DoubleArgmaxMonoid = ArgmaxMonoid<double, []() { return -DINF; }>;
