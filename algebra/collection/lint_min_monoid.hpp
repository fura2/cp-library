#pragma once

#include <algorithm>

#include "algebra/monoid_impl.hpp"
#include "template/constant.hpp"

using LintMinMonoid =
    MonoidImpl<long long,
               [](long long a, long long b) { return std::min(a, b); },
               []() { return LINF; }>;
