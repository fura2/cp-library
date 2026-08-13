#pragma once

#include "algebra/semiring_impl.hpp"
#include "template/constant.hpp"

using IntMinPlusSemiring =
    SemiringImpl<int,
                 [](int a, int b) { return b < a ? b : a; },
                 [](int a, int b) {
                   if (a == INF || b == INF) return INF;
                   return a + b;
                 },
                 []() { return INF; },
                 []() { return 0; }>;
