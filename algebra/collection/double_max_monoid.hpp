#pragma once

#include "algebra/collection/max_monoid.hpp"
#include "template/constant.hpp"

using DoubleMaxMonoid = MaxMonoid<double, []() { return -DINF; }>;
