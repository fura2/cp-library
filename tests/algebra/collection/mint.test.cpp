#include "mint/mint.hpp"

#include <cassert>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/add_monoid.hpp"
#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/add_mul_semiring.hpp"
#include "algebra/collection/mul_monoid.hpp"
#include "algebra/group.hpp"
#include "algebra/ring.hpp"

static_assert(Group<AddGroup<mint>> && Monoid<MulMonoid<mint>>);
static_assert(Monoid<AddMonoid<mint>> && !Group<AddMonoid<mint>>);
static_assert(Ring<AddMulRing<mint>>);
static_assert(Semiring<AddMulSemiring<mint>> && !Ring<AddMulSemiring<mint>>);

int main() {
  assert(id_zero<mint>().unwrap() == 0 && id_one<mint>().unwrap() == 1);
  assert(AddMulRing<mint>::zero().unwrap().unwrap() == 0);
  assert(AddMulRing<mint>::one().unwrap().unwrap() == 1);
  const AddMulRing<mint> a{mint{-1}}, b{mint{2}};
  assert((a + b).unwrap().unwrap() == 1);
  assert((b - a).unwrap().unwrap() == 3);
  assert((a * a).unwrap().unwrap() == 1);
  assert((-a).unwrap().unwrap() == 1);
  assert(AddGroup<mint>::identity().unwrap().unwrap() == 0);
  assert(MulMonoid<mint>::identity().unwrap().unwrap() == 1);
  const mint minus_one{-1}, two{2};
  assert(AddGroup<mint>{}.unwrap().unwrap() == 0);
  assert(MulMonoid<mint>{}.unwrap().unwrap() == 1);
  assert((AddGroup<mint>{minus_one} * AddGroup<mint>{two}).unwrap().unwrap() ==
         1);
  assert(AddGroup<mint>{minus_one}.inverse().unwrap().unwrap() == 1);
  assert((MulMonoid<mint>{minus_one} * MulMonoid<mint>{minus_one})
             .unwrap()
             .unwrap() == 1);
  assert(AddMonoid<mint>{}.unwrap().unwrap() == 0);
  assert(
      (AddMonoid<mint>{minus_one} * AddMonoid<mint>{two}).unwrap().unwrap() ==
      1);
  using S = AddMulSemiring<mint>;
  assert(S{}.unwrap().unwrap() == 0 && S::zero().unwrap().unwrap() == 0);
  assert(S::one().unwrap().unwrap() == 1);
  assert((S{minus_one} + S{two}).unwrap().unwrap() == 1);
  assert((S{minus_one} * S{minus_one}).unwrap().unwrap() == 1);
}
