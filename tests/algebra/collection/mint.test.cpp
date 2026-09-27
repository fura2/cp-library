#include <cassert>
#include <concepts>

#include "algebra/collection/mint_add_group.hpp"
#include "algebra/collection/mint_add_monoid.hpp"
#include "algebra/collection/mint_add_mul_ring.hpp"
#include "algebra/collection/mint_add_mul_semiring.hpp"
#include "algebra/collection/mint_mul_monoid.hpp"
#include "algebra/group.hpp"
#include "algebra/ring.hpp"

static_assert(std::same_as<MintAddGroup, AddGroup<mint>>);
static_assert(std::same_as<MintAddMonoid, AddMonoid<mint>>);
static_assert(std::same_as<MintMulMonoid, MulMonoid<mint>>);
static_assert(std::same_as<MintAddMulRing, AddMulRing<mint>>);
static_assert(std::same_as<MintAddMulSemiring, AddMulSemiring<mint>>);
static_assert(Group<MintAddGroup> && Monoid<MintMulMonoid>);
static_assert(Monoid<MintAddMonoid> && !Group<MintAddMonoid>);
static_assert(Ring<MintAddMulRing>);
static_assert(Semiring<MintAddMulSemiring> && !Ring<MintAddMulSemiring>);

int main() {
  assert(id_zero<mint>().unwrap() == 0 && id_one<mint>().unwrap() == 1);
  assert(MintAddMulRing::zero().unwrap().unwrap() == 0);
  assert(MintAddMulRing::one().unwrap().unwrap() == 1);
  const MintAddMulRing a{mint{-1}}, b{mint{2}};
  assert((a + b).unwrap().unwrap() == 1);
  assert((b - a).unwrap().unwrap() == 3);
  assert((a * a).unwrap().unwrap() == 1);
  assert((-a).unwrap().unwrap() == 1);
  assert(MintAddGroup::identity().unwrap().unwrap() == 0);
  assert(MintMulMonoid::identity().unwrap().unwrap() == 1);
  const mint minus_one{-1}, two{2};
  assert(MintAddGroup{}.unwrap().unwrap() == 0);
  assert(MintMulMonoid{}.unwrap().unwrap() == 1);
  assert((MintAddGroup{minus_one} * MintAddGroup{two}).unwrap().unwrap() == 1);
  assert(MintAddGroup{minus_one}.inverse().unwrap().unwrap() == 1);
  assert(
      (MintMulMonoid{minus_one} * MintMulMonoid{minus_one}).unwrap().unwrap() ==
      1);
  assert(MintAddMonoid{}.unwrap().unwrap() == 0);
  assert((MintAddMonoid{minus_one} * AddMonoid<mint>{two}).unwrap().unwrap() ==
         1);
  using S = MintAddMulSemiring;
  assert(S{}.unwrap().unwrap() == 0 && S::zero().unwrap().unwrap() == 0);
  assert(S::one().unwrap().unwrap() == 1);
  assert((S{minus_one} + S{two}).unwrap().unwrap() == 1);
  assert((S{minus_one} * S{minus_one}).unwrap().unwrap() == 1);
}
