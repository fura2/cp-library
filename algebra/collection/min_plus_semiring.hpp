#pragma once

#include <concepts>

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/semiring_impl.hpp"

namespace min_plus_semiring_detail {

template <typename T>
  requires requires {
    { id_inf<T>() } -> std::same_as<T>;
  } && requires(const T& a, const T& b) {
    { a + b } -> std::same_as<T>;
    { a == b } -> std::same_as<bool>;
  }
inline constexpr auto op = [](const T& a, const T& b) -> T {
  if (a == id_inf<T>() || b == id_inf<T>()) return id_inf<T>();
  return a + b;
};

}  // namespace min_plus_semiring_detail

template <typename T>
using MinPlusSemiring = SemiringImpl<T,
                                     op_min<T>,
                                     min_plus_semiring_detail::op<T>,
                                     id_inf<T>,
                                     id_zero<T>>;
