#pragma once

#include "algebra/collection/min_monoid.hpp"
#include "template/constant.hpp"

using LintMinMonoid = MinMonoid<long long, []() { return LINF; }>;
