#pragma once

#include "algebra/collection/id.hpp"
#include "algebra/semiring.hpp"
#include "linear_algebra/fixed_square_matrix.hpp"

template <int N, Semiring S>
inline constexpr auto id_zero<FixedSquareMatrix<N, S>> =
    []() { return FixedSquareMatrix<N, S>::zero(); };

template <int N, Semiring S>
inline constexpr auto id_one<FixedSquareMatrix<N, S>> =
    []() { return FixedSquareMatrix<N, S>::identity(); };
