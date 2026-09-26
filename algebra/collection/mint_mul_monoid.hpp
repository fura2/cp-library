#pragma once

#include "algebra/collection/mul_monoid.hpp"
#include "mint/mint.hpp"

using MintMulMonoid = MulMonoid<mint, []() { return mint(1); }>;
