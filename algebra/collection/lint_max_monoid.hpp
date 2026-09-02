#pragma once

#include "algebra/collection/max_monoid.hpp"
#include "template/constant.hpp"

using LintMaxMonoid = MaxMonoid<long long, []() { return -LINF; }>;
