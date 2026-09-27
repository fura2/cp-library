#pragma once

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/semiring_impl.hpp"

template <typename T>
using AddMulSemiring =
    SemiringImpl<T, op_add<T>, op_mul<T>, id_zero<T>, id_one<T>>;
