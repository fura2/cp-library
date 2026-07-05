#pragma once

#include <cstddef>

#include "algebra/semiring.hpp"
#include "linear_algebra/square_matrix.hpp"

template <Semiring R, std::size_t N>
struct MatrixSemiring {
  using value_type = SquareMatrix<R, N>;
  static constexpr value_type add(const value_type& a, const value_type& b) {
    return a + b;
  }
  static constexpr value_type mul(const value_type& a, const value_type& b) {
    return a * b;
  }
  static constexpr value_type zero() { return value_type::zero(); }
  static constexpr value_type one() { return value_type::identity(); }
};
