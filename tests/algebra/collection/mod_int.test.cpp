#include "mint/mod_int.hpp"

#include <cassert>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/add_monoid.hpp"
#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/add_mul_semiring.hpp"
#include "algebra/collection/mul_monoid.hpp"
#include "algebra/group.hpp"
#include "algebra/ring.hpp"

using Mint = ModInt<7>;

static_assert(Group<AddGroup<Mint>> && Monoid<MulMonoid<Mint>>);
static_assert(Monoid<AddMonoid<Mint>> && !Group<AddMonoid<Mint>>);
static_assert(Ring<AddMulRing<Mint>>);
static_assert(Semiring<AddMulSemiring<Mint>> && !Ring<AddMulSemiring<Mint>>);

int main() {
  assert(id_zero<Mint>().unwrap() == 0 && id_one<Mint>().unwrap() == 1);
  assert(AddMulRing<Mint>::zero().unwrap().unwrap() == 0);
  assert(AddMulRing<Mint>::one().unwrap().unwrap() == 1);
  const AddMulRing<Mint> a{Mint{-1}}, b{Mint{2}};
  assert((a + b).unwrap().unwrap() == 1);
  assert((b - a).unwrap().unwrap() == 3);
  assert((a * a).unwrap().unwrap() == 1);
  assert((-a).unwrap().unwrap() == 1);
  assert(AddGroup<Mint>::identity().unwrap().unwrap() == 0);
  assert(MulMonoid<Mint>::identity().unwrap().unwrap() == 1);
  const Mint minus_one{-1}, two{2};
  assert(AddGroup<Mint>{}.unwrap().unwrap() == 0);
  assert(MulMonoid<Mint>{}.unwrap().unwrap() == 1);
  assert((AddGroup<Mint>{minus_one} * AddGroup<Mint>{two}).unwrap().unwrap() ==
         1);
  assert(AddGroup<Mint>{minus_one}.inverse().unwrap().unwrap() == 1);
  assert((MulMonoid<Mint>{minus_one} * MulMonoid<Mint>{minus_one})
             .unwrap()
             .unwrap() == 1);
  assert(AddMonoid<Mint>{}.unwrap().unwrap() == 0);
  assert(
      (AddMonoid<Mint>{minus_one} * AddMonoid<Mint>{two}).unwrap().unwrap() ==
      1);
  using S = AddMulSemiring<Mint>;
  assert(S{}.unwrap().unwrap() == 0 && S::zero().unwrap().unwrap() == 0);
  assert(S::one().unwrap().unwrap() == 1);
  assert((S{minus_one} + S{two}).unwrap().unwrap() == 1);
  assert((S{minus_one} * S{minus_one}).unwrap().unwrap() == 1);
}
