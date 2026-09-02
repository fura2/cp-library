#pragma once

#include "algebra/collection/min_monoid.hpp"
#include "template/constant.hpp"

using DoubleMinMonoid = MinMonoid<double, []() { return DINF; }>;
