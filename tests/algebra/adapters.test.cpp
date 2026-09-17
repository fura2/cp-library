// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include <cassert>
#include <utility>

#include "algebra/additive_group_of_ring.hpp"
#include "algebra/additive_monoid_of_semiring.hpp"
#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/min_plus_semiring.hpp"
#include "algebra/multiplicative_monoid_of_semiring.hpp"

int main() {
  using Add = AdditiveMonoidOfSemiring<IntMinPlusSemiring>;
  using Mul = MultiplicativeMonoidOfSemiring<IntMinPlusSemiring>;
  using Group = AdditiveGroupOfRing<IntRing>;
  const IntMinPlusSemiring value{3};
  Add a{value};
  Mul m{value};
  const IntRing ring_value{3};
  Group g{ring_value};
  assert(Add{}.unwrap() == INF && Add::identity().unwrap() == INF);
  assert(Mul{}.unwrap() == 0 && Mul::identity().unwrap() == 0);
  assert(Group{}.unwrap() == 0 && Group::identity().unwrap() == 0);
  assert((a * Add{IntMinPlusSemiring{5}}).unwrap() == 3);
  assert((m * Mul{IntMinPlusSemiring{5}}).unwrap() == 8);
  assert((g * Group{IntRing{5}}).unwrap() == 8);
  assert(g.inverse().unwrap() == -3 && (g * g.inverse()).unwrap() == 0);
  assert((a * Add::identity()).unwrap() == 3);
  assert((Mul::identity() * m).unwrap() == 3);
  a.unwrap() = 7;
  m.unwrap() = 8;
  g.unwrap() = 9;
  assert(std::as_const(a).unwrap() == 7 && pretty(a) == "7");
  assert(std::as_const(m).unwrap() == 8 && pretty(m) == "8");
  assert(std::as_const(g).unwrap() == 9 && pretty(g) == "9");
}
