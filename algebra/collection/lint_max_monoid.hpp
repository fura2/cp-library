#pragma once

#include "algebra/monoid_impl.hpp"
#include "template/constant.hpp"

using LintMaxMonoid =
    MonoidImpl<long long,
               [](long long a, long long b) { return b > a ? b : a; },
               []() { return -LINF; }>;
