#pragma once

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/ring_impl.hpp"

template <typename T>
using AddMulRing =
    RingImpl<T, op_add<T>, op_mul<T>, id_zero<T>, id_one<T>, op_neg<T>>;
