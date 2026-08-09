#pragma once

#include <algorithm>

#include "algebra/monoid.hpp"
#include "template/constant.hpp"

using IntMinMonoid = MonoidImpl<int,
                                [](int a, int b) { return std::min(a, b); },
                                []() { return INF; }>;
