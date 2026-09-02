#pragma once

#include <concepts>

#include "algebra/monoid_impl.hpp"

template <typename T, auto Id>
  requires requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
using MaxMonoid =
    MonoidImpl<T, [](const T& a, const T& b) { return a < b ? b : a; }, Id>;
