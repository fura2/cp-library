#include <cassert>
#include <string>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/add_mul_ring.hpp"
#include "algebra/collection/min_monoid.hpp"
#include "algebra/collection/min_plus_semiring.hpp"
#include "algebra/group.hpp"
#include "algebra/monoid.hpp"
#include "algebra/monoid_action.hpp"
#include "algebra/ring.hpp"
#include "algebra/semigroup.hpp"
#include "algebra/semiring.hpp"

struct AddAction {
  using value_monoid = IntMinMonoid;
  using action_monoid = IntAddGroup;
  static value_monoid act(const value_monoid& x, const action_monoid& f) {
    return x.unwrap() == INF ? x : value_monoid{x.unwrap() + f.unwrap()};
  }
};

struct MissingAct {
  using value_monoid = IntMinMonoid;
  using action_monoid = IntAddGroup;
};

static_assert(Semigroup<int> && !Monoid<int>);
static_assert(!Semigroup<std::string> && !Group<int>);
static_assert(Monoid<IntMinMonoid> && IdempotentSemigroup<IntMinMonoid>);
static_assert(!Group<IntMinMonoid>);
static_assert(Group<IntAddGroup> && CommutativeMonoid<IntAddGroup>);
static_assert(Semiring<IntMinPlusSemiring> && !Ring<IntMinPlusSemiring>);
static_assert(Ring<IntRing> && Semiring<IntRing>);
static_assert(!Semiring<int> && !Ring<int>);
static_assert(MonoidAction<AddAction>);
static_assert(!MonoidAction<MissingAct> && !MonoidAction<int>);

int main() {
  assert(AddAction::act(IntMinMonoid{3}, IntAddGroup{2}).unwrap() == 5);
  assert(AddAction::act(IntMinMonoid::identity(), IntAddGroup{2}).unwrap() ==
         INF);
}
