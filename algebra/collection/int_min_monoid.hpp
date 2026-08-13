#pragma once

#include "algebra/monoid_impl.hpp"
#include "template/constant.hpp"

using IntMinMonoid = MonoidImpl<int,
                                [](int a, int b) { return b < a ? b : a; },
                                []() { return INF; }>;
