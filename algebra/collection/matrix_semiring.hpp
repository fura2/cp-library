#pragma once

#include "algebra/collection/add_mul_semiring.hpp"
#include "algebra/collection/matrix_id.hpp"
#include "algebra/semiring.hpp"
#include "linear_algebra/fixed_square_matrix.hpp"

template <int N, Semiring S>
using MatrixSemiring = AddMulSemiring<FixedSquareMatrix<N, S>>;
