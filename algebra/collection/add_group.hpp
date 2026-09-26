#pragma once

#include <concepts>

#include "algebra/group_impl.hpp"

namespace add_group_detail {

template <typename T>
inline constexpr auto add = [](const T& a, const T& b) -> T { return a + b; };

template <typename T>
inline constexpr auto neg = [](const T& a) -> T { return -a; };

}  // namespace add_group_detail

template <typename T, auto Zero>
  requires(
              requires {
                { Zero() } -> std::same_as<T>;
              } &&
              requires(const T& a, const T& b) {
                { a + b } -> std::same_as<T>;
              } &&
              requires(const T& a) {
                { -a } -> std::same_as<T>;
              })
using AddGroup =
    GroupImpl<T, add_group_detail::add<T>, Zero, add_group_detail::neg<T>>;

using IntAddGroup = AddGroup<int, []() { return 0; }>;
using LintAddGroup = AddGroup<long long, []() { return 0LL; }>;
using DoubleAddGroup = AddGroup<double, []() { return 0.0; }>;
