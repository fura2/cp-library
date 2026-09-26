#include <cassert>
#include <string>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/add_monoid.hpp"
#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/mint_add_group.hpp"
#include "algebra/collection/mint_mul_monoid.hpp"
#include "algebra/collection/mint_ring.hpp"
#include "algebra/collection/mul_monoid.hpp"
#include "algebra/group.hpp"
#include "algebra/monoid.hpp"

template <typename T, auto Zero>
concept ValidAddMonoid = requires { typename AddMonoid<T, Zero>; };

template <typename T, auto Zero>
concept ValidAddGroup = requires { typename AddGroup<T, Zero>; };

template <typename T, auto One>
concept ValidMulMonoid = requires { typename MulMonoid<T, One>; };

template <typename T, auto Zero, auto One>
concept ValidAddMulRing = requires { typename AddMulRing<T, Zero, One>; };

inline constexpr auto zero = []() { return 0; };
inline constexpr auto one = []() { return 1; };
inline constexpr auto lint_zero = []() { return 0LL; };
inline constexpr auto empty_string = []() { return std::string{}; };
using Concat = AddMonoid<std::string, empty_string>;

static_assert(ValidAddMonoid<int, zero> && ValidAddGroup<int, zero>);
static_assert(ValidMulMonoid<int, one> && ValidAddMulRing<int, zero, one>);
static_assert(!ValidAddMonoid<int, lint_zero>);
static_assert(!ValidAddGroup<int, lint_zero>);
static_assert(!ValidMulMonoid<int, lint_zero>);
static_assert(!ValidAddMulRing<int, lint_zero, one>);
static_assert(!ValidAddMulRing<int, zero, lint_zero>);
static_assert(Monoid<Concat> && !Group<Concat>);
static_assert(!ValidAddGroup<std::string, empty_string>);
static_assert(!ValidMulMonoid<std::string, empty_string>);

template <typename R, typename A, typename M>
void check_numeric() {
  assert(R::zero().unwrap() == 0 && R::one().unwrap() == 1);
  assert((R{3} + R{4}).unwrap() == 7);
  assert((R{3} - R{4}).unwrap() == -1);
  assert((R{3} * R{4}).unwrap() == 12 && (-R{3}).unwrap() == -3);
  assert(A{}.unwrap() == 0 && M{}.unwrap() == 1);
  assert(A::identity().unwrap() == 0 && M::identity().unwrap() == 1);
  assert((A{3} * A{4}).unwrap() == 7);
  assert((M{3} * M{4}).unwrap() == 12);
  assert(A{3}.inverse().unwrap() == -3);
  assert((A{3} * A{3}.inverse()).unwrap() == 0);
}

int main() {
  check_numeric<IntRing, IntAddGroup, IntMulMonoid>();
  check_numeric<LintRing, LintAddGroup, LintMulMonoid>();
  check_numeric<DoubleRing, DoubleAddGroup, DoubleMulMonoid>();
  assert((LintRing{3'000'000'000LL} + LintRing{4'000'000'000LL}).unwrap() ==
         7'000'000'000LL);
  assert((DoubleRing{0.5} * DoubleRing{0.25}).unwrap() == 0.125);
  assert((LintAddGroup{3'000'000'000LL} * LintAddGroup{4'000'000'000LL})
             .unwrap() == 7'000'000'000LL);
  assert((LintMulMonoid{3'000'000'000LL} * LintMulMonoid{2}).unwrap() ==
         6'000'000'000LL);
  assert((DoubleAddGroup{0.5} * DoubleAddGroup{0.25}).unwrap() == 0.75);
  assert((DoubleMulMonoid{0.5} * DoubleMulMonoid{0.25}).unwrap() == 0.125);

  const Concat first{std::string{"ab"}}, last{std::string{"cd"}};
  assert(Concat{}.unwrap().empty());
  assert((first * last).unwrap() == "abcd");
  assert((last * first).unwrap() == "cdab");
  assert((first * Concat::identity()).unwrap() == "ab");
  assert((Concat::identity() * first).unwrap() == "ab");

  assert(MintRing::zero().unwrap().unwrap() == 0);
  assert(MintRing::one().unwrap().unwrap() == 1);
  const MintRing a{mint{-1}}, b{mint{2}};
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
      (MintMulMonoid{minus_one} * MintMulMonoid{mint{-1}}).unwrap().unwrap() ==
      1);
}
