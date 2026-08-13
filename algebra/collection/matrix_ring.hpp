#pragma once

#include <cstddef>

#include "algebra/ring.hpp"
#include "algebra/ring_impl.hpp"
#include "linear_algebra/fixed_square_matrix.hpp"

namespace matrix_ring_detail {

template <std::size_t N, Ring R>
inline constexpr auto add =
    [](const FixedSquareMatrix<N, R>& a, const FixedSquareMatrix<N, R>& b) {
      return a + b;
    };

template <std::size_t N, Ring R>
inline constexpr auto mul =
    [](const FixedSquareMatrix<N, R>& a, const FixedSquareMatrix<N, R>& b) {
      return a * b;
    };

template <std::size_t N, Ring R>
inline constexpr auto zero = []() { return FixedSquareMatrix<N, R>::zero(); };

template <std::size_t N, Ring R>
inline constexpr auto one =
    []() { return FixedSquareMatrix<N, R>::identity(); };

template <std::size_t N, Ring R>
inline constexpr auto neg = [](const FixedSquareMatrix<N, R>& a) { return -a; };

}  // namespace matrix_ring_detail

template <std::size_t N, Ring R>
using MatrixRing = RingImpl<FixedSquareMatrix<N, R>,
                            matrix_ring_detail::add<N, R>,
                            matrix_ring_detail::mul<N, R>,
                            matrix_ring_detail::zero<N, R>,
                            matrix_ring_detail::one<N, R>,
                            matrix_ring_detail::neg<N, R>>;
