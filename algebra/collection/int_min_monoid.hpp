#pragma once

#include "algebra/collection/min_monoid.hpp"
#include "template/constant.hpp"

using IntMinMonoid = MinMonoid<int, []() { return INF; }>;
