#pragma once

#include <algorithm>

#include "algebra/semiring.hpp"
#include "template/constant.hpp"

using MinPlusSemiring =
    SemiringImpl<long long,
                 [](long long a, long long b) { return std::min(a, b); },
                 [](long long a, long long b) {
                   if (a == LINF || b == LINF) return LINF;
                   return a + b;
                 },
                 []() { return LINF; },
                 []() { return 0LL; }>;
