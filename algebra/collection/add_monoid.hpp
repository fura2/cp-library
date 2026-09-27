#pragma once

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/monoid_impl.hpp"

template <typename T>
using AddMonoid = MonoidImpl<T, op_add<T>, id_zero<T>>;
