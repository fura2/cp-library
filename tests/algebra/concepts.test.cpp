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
  using value_type = MinMonoid<int>;
  using action_type = AddGroup<int>;
  static value_type act(const value_type& x, const action_type& f) {
    return x.unwrap() == INF ? x : value_type{x.unwrap() + f.unwrap()};
  }
};

struct ScalarAddAction {
  using value_type = int;
  using action_type = AddGroup<int>;
  static value_type act(const value_type& x, const action_type& f) {
    return x + f.unwrap();
  }
};

struct MissingAct {
  using value_type = MinMonoid<int>;
  using action_type = AddGroup<int>;
};

struct NonMonoidAction {
  using value_type = int;
  using action_type = int;
  static value_type act(const value_type& x, const action_type& f) {
    return x + f;
  }
};

static_assert(Semigroup<int> && !Monoid<int>);
static_assert(!Semigroup<std::string> && !Group<int>);
static_assert(Monoid<MinMonoid<int>> && IdempotentSemigroup<MinMonoid<int>>);
static_assert(!Group<MinMonoid<int>>);
static_assert(Group<AddGroup<int>> && CommutativeMonoid<AddGroup<int>>);
static_assert(Semiring<MinPlusSemiring<int>> && !Ring<MinPlusSemiring<int>>);
static_assert(Ring<AddMulRing<int>> && Semiring<AddMulRing<int>>);
static_assert(!Semiring<int> && !Ring<int>);
static_assert(MonoidAction<AddAction> && MonoidActionOnMonoid<AddAction>);
static_assert(MonoidAction<ScalarAddAction>);
static_assert(!MonoidActionOnMonoid<ScalarAddAction>);
static_assert(!MonoidAction<NonMonoidAction>);
static_assert(!MonoidActionOnMonoid<NonMonoidAction>);
static_assert(!MonoidAction<MissingAct> && !MonoidAction<int>);
static_assert(!MonoidActionOnMonoid<MissingAct> && !MonoidActionOnMonoid<int>);

int main() {}
