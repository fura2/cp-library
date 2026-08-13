#pragma once

#include "algebra/monoid_impl.hpp"
#include "template/constant.hpp"

using DoubleMaxMonoid =
    MonoidImpl<double,
               [](double a, double b) { return b > a ? b : a; },
               []() { return -DINF; }>;
