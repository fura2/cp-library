#pragma once

#include <concepts>

#include "algebra/monoid_impl.hpp"

template <typename T, auto Id>
  requires requires(const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
  }
using MinMonoid =
    MonoidImpl<T, [](const T& a, const T& b) { return b < a ? b : a; }, Id>;
