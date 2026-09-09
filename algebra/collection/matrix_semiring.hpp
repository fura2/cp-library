#pragma once

#include "algebra/semiring.hpp"
#include "algebra/semiring_impl.hpp"
#include "linear_algebra/fixed_square_matrix.hpp"

namespace matrix_semiring_detail {

template <int N, Semiring S>
inline constexpr auto add =
    [](const FixedSquareMatrix<N, S>& a, const FixedSquareMatrix<N, S>& b) {
      return a + b;
    };

template <int N, Semiring S>
inline constexpr auto mul =
    [](const FixedSquareMatrix<N, S>& a, const FixedSquareMatrix<N, S>& b) {
      return a * b;
    };

template <int N, Semiring S>
inline constexpr auto zero = []() { return FixedSquareMatrix<N, S>::zero(); };

template <int N, Semiring S>
inline constexpr auto one =
    []() { return FixedSquareMatrix<N, S>::identity(); };

}  // namespace matrix_semiring_detail

template <int N, Semiring S>
using MatrixSemiring = SemiringImpl<FixedSquareMatrix<N, S>,
                                    matrix_semiring_detail::add<N, S>,
                                    matrix_semiring_detail::mul<N, S>,
                                    matrix_semiring_detail::zero<N, S>,
                                    matrix_semiring_detail::one<N, S>>;
