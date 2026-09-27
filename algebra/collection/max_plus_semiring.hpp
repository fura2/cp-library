#pragma once

#include <concepts>

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/semiring_impl.hpp"

namespace max_plus_semiring_detail {

template <typename T>
  requires requires {
    { id_neg_inf<T>() } -> std::same_as<T>;
  } && requires(const T& a, const T& b) {
    { a + b } -> std::same_as<T>;
    { a == b } -> std::same_as<bool>;
  }
inline constexpr auto op = [](const T& a, const T& b) -> T {
  if (a == id_neg_inf<T>() || b == id_neg_inf<T>()) return id_neg_inf<T>();
  return a + b;
};

}  // namespace max_plus_semiring_detail

template <typename T>
using MaxPlusSemiring = SemiringImpl<T,
                                     op_max<T>,
                                     max_plus_semiring_detail::op<T>,
                                     id_neg_inf<T>,
                                     id_zero<T>>;
