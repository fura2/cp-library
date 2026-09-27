#pragma once

#include <concepts>

#include "template/constant.hpp"

template <typename T>
inline constexpr auto id_zero = nullptr;
template <>
inline constexpr auto id_zero<int> = []() { return 0; };
template <>
inline constexpr auto id_zero<long long> = []() { return 0LL; };
template <>
inline constexpr auto id_zero<double> = []() { return 0.0; };

template <typename T>
inline constexpr auto id_one = nullptr;
template <>
inline constexpr auto id_one<int> = []() { return 1; };
template <>
inline constexpr auto id_one<long long> = []() { return 1LL; };
template <>
inline constexpr auto id_one<double> = []() { return 1.0; };

template <typename T>
inline constexpr auto id_inf = nullptr;
template <>
inline constexpr auto id_inf<int> = []() { return INF; };
template <>
inline constexpr auto id_inf<long long> = []() { return LINF; };
template <>
inline constexpr auto id_inf<double> = []() { return DINF; };

template <typename T>
  requires requires() {
    { id_inf<T>() } -> std::same_as<T>;
  } && requires(const T& a) {
    { -a } -> std::same_as<T>;
  }
inline constexpr auto id_neg_inf = []() -> T { return -id_inf<T>(); };
