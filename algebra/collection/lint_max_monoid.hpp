#pragma once

#include <algorithm>

#include "algebra/monoid_impl.hpp"
#include "template/constant.hpp"

using LintMaxMonoid =
    MonoidImpl<long long,
               [](long long a, long long b) { return std::max(a, b); },
               []() { return -LINF; }>;
