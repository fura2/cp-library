#pragma once

#include "algebra/collection/max_monoid.hpp"
#include "template/constant.hpp"

using IntMaxMonoid = MaxMonoid<int, []() { return -INF; }>;
