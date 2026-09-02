#pragma once

#include "algebra/semiring_impl.hpp"
#include "template/constant.hpp"

using DoubleMaxPlusSemiring =
    SemiringImpl<double,
                 [](double a, double b) { return a < b ? b : a; },
                 [](double a, double b) {
                   if (a == -DINF || b == -DINF) return -DINF;
                   return a + b;
                 },
                 []() { return -DINF; },
                 []() { return 0.0; }>;
