#pragma once

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/monoid_impl.hpp"

template <typename T>
using MinMonoid = MonoidImpl<T, op_min<T>, id_inf<T>>;
