#pragma once

#include "algebra/collection/add_mul_ring.hpp"
#include "mint/mint.hpp"

using MintAddMulRing =
    AddMulRing<mint, []() { return mint(0); }, []() { return mint(1); }>;

using MintRing = MintAddMulRing;
