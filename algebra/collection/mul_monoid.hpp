#pragma once

#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/multiplicative_monoid_of_semiring.hpp"

using IntMulMonoid = MultiplicativeMonoidOfSemiring<IntRing>;
using LintMulMonoid = MultiplicativeMonoidOfSemiring<LintRing>;
using DoubleMulMonoid = MultiplicativeMonoidOfSemiring<DoubleRing>;
