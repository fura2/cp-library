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
  using value_monoid = MinMonoid<int>;
  using action_monoid = AddGroup<int>;
  static value_monoid act(const value_monoid& x, const action_monoid& f) {
    return x.unwrap() == INF ? x : value_monoid{x.unwrap() + f.unwrap()};
  }
};

struct MissingAct {
  using value_monoid = MinMonoid<int>;
  using action_monoid = AddGroup<int>;
};

static_assert(Semigroup<int> && !Monoid<int>);
static_assert(!Semigroup<std::string> && !Group<int>);
static_assert(Monoid<MinMonoid<int>> && IdempotentSemigroup<MinMonoid<int>>);
static_assert(!Group<MinMonoid<int>>);
static_assert(Group<AddGroup<int>> && CommutativeMonoid<AddGroup<int>>);
static_assert(Semiring<MinPlusSemiring<int>> && !Ring<MinPlusSemiring<int>>);
static_assert(Ring<AddMulRing<int>> && Semiring<AddMulRing<int>>);
static_assert(!Semiring<int> && !Ring<int>);
static_assert(MonoidAction<AddAction>);
static_assert(!MonoidAction<MissingAct> && !MonoidAction<int>);

int main() {
  assert(AddAction::act(MinMonoid<int>{3}, AddGroup<int>{2}).unwrap() == 5);
  assert(
      AddAction::act(MinMonoid<int>::identity(), AddGroup<int>{2}).unwrap() ==
      INF);
}
