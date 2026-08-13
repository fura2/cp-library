#pragma once

#include <algorithm>

#include "algebra/monoid_impl.hpp"
#include "template/constant.hpp"

using IntMaxMonoid = MonoidImpl<int,
                                [](int a, int b) { return std::max(a, b); },
                                []() { return -INF; }>;
