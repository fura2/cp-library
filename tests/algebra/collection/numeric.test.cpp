#include <cassert>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/mint_add_group.hpp"
#include "algebra/collection/mint_mul_monoid.hpp"
#include "algebra/collection/mint_ring.hpp"
#include "algebra/collection/mul_monoid.hpp"

template <typename R, typename A, typename M>
void check_numeric() {
  assert(R::zero().unwrap() == 0 && R::one().unwrap() == 1);
  assert((R{3} + R{4}).unwrap() == 7);
  assert((R{3} - R{4}).unwrap() == -1);
  assert((R{3} * R{4}).unwrap() == 12 && (-R{3}).unwrap() == -3);
  assert(A::identity().unwrap() == 0 && M::identity().unwrap() == 1);
  assert((A{R{3}} * A{R{4}}).unwrap() == 7);
  assert((M{R{3}} * M{R{4}}).unwrap() == 12);
  assert(A{R{3}}.inverse().unwrap() == -3);
}

int main() {
  check_numeric<IntRing, IntAddGroup, IntMulMonoid>();
  check_numeric<LintRing, LintAddGroup, LintMulMonoid>();
  check_numeric<DoubleRing, DoubleAddGroup, DoubleMulMonoid>();
  assert((LintRing{3'000'000'000LL} + LintRing{4'000'000'000LL}).unwrap() ==
         7'000'000'000LL);
  assert((DoubleRing{0.5} * DoubleRing{0.25}).unwrap() == 0.125);
  assert(MintRing::zero().unwrap().unwrap() == 0);
  assert(MintRing::one().unwrap().unwrap() == 1);
  const MintRing a{mint{-1}}, b{mint{2}};
  assert((a + b).unwrap().unwrap() == 1);
  assert((b - a).unwrap().unwrap() == 3);
  assert((a * a).unwrap().unwrap() == 1);
  assert((-a).unwrap().unwrap() == 1);
  assert(MintAddGroup::identity().unwrap().unwrap() == 0);
  assert(MintMulMonoid::identity().unwrap().unwrap() == 1);
  assert((MintAddGroup{a} * MintAddGroup{b}).unwrap().unwrap() == 1);
  assert(MintAddGroup{a}.inverse().unwrap().unwrap() == 1);
  assert((MintMulMonoid{a} * MintMulMonoid{a}).unwrap().unwrap() == 1);
}
