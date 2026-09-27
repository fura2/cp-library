#pragma once

#include <concepts>
#include <functional>

template <typename T>
  requires requires(const T& a, const T& b) {
    { a + b } -> std::same_as<T>;
  }
inline constexpr auto op_add = std::plus<T>{};

template <typename T>
  requires requires(const T& a, const T& b) {
    { a * b } -> std::same_as<T>;
  }
inline constexpr auto op_mul = std::multiplies<T>{};

template <typename T>
  requires requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
inline constexpr auto op_max =
    [](const T& a, const T& b) -> T { return a < b ? b : a; };

template <typename T>
  requires requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
inline constexpr auto op_min =
    [](const T& a, const T& b) -> T { return b < a ? b : a; };

template <typename T>
  requires requires(const T& a) {
    { -a } -> std::same_as<T>;
  }
inline constexpr auto op_neg = std::negate<T>{};
