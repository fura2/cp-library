#pragma once

#include "algebra/semiring_impl.hpp"
#include "template/constant.hpp"

using LintMaxPlusSemiring =
    SemiringImpl<long long,
                 [](long long a, long long b) { return a < b ? b : a; },
                 [](long long a, long long b) {
                   if (a == -LINF || b == -LINF) return -LINF;
                   return a + b;
                 },
                 []() { return -LINF; },
                 []() { return 0LL; }>;
