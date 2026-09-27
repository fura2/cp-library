#pragma once

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/monoid_impl.hpp"

template <typename T>
using MulMonoid = MonoidImpl<T, op_mul<T>, id_one<T>>;
