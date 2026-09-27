#pragma once

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/monoid_impl.hpp"

template <typename T>
using MaxMonoid = MonoidImpl<T, op_max<T>, id_neg_inf<T>>;
