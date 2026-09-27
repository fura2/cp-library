#pragma once

#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/matrix_id.hpp"
#include "algebra/ring.hpp"
#include "linear_algebra/fixed_square_matrix.hpp"

template <int N, Ring R>
using MatrixRing = AddMulRing<FixedSquareMatrix<N, R>>;
