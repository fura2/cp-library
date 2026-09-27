#pragma once

#include "algebra/collection/id.hpp"
#include "algebra/collection/op.hpp"
#include "algebra/group_impl.hpp"

template <typename T>
using AddGroup = GroupImpl<T, op_add<T>, id_zero<T>, op_neg<T>>;
