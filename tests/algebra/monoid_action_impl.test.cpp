#include "algebra/monoid_action_impl.hpp"

#include <cassert>
#include <concepts>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/mul_monoid.hpp"
#include "algebra/monoid_action.hpp"

using M = IntAddGroup;
using F = IntMulMonoid;

M scale(const M& m, const F& f) { return M{m.unwrap() * f.unwrap()}; }

using ScaleSum = MonoidActionImpl<M, F, scale>;
using GenericScaleSum =
    MonoidActionImpl<M, F, [](const auto& m, const auto& f) {
      return M{m.unwrap() * f.unwrap()};
    }>;

template <typename V, typename U, auto Act>
concept ValidActionImpl = requires { typename MonoidActionImpl<V, U, Act>; };

static_assert(MonoidAction<ScaleSum> && MonoidAction<GenericScaleSum>);
static_assert(std::same_as<ScaleSum::value_monoid, M>);
static_assert(std::same_as<ScaleSum::action_monoid, F>);
static_assert(!ValidActionImpl<int, F, scale>);
static_assert(!ValidActionImpl<M, int, scale>);
static_assert(!ValidActionImpl<M, F, [](const M&, const F&) { return 0; }>);
static_assert(
    !ValidActionImpl<M, F, [](const M& m, const F&) -> const M& { return m; }>);
static_assert(!ValidActionImpl<M, F, [](M& m, F&) { return m; }>);

template <MonoidAction A>
void check_action() {
  const M m{3}, n{5};
  const F f{2}, g{4};
  assert(A::act(m, f).unwrap() == 6);
  assert(A::act(m, F::identity()).unwrap() == 3);
  assert(A::act(M::identity(), f).unwrap() == 0);
  assert(A::act(m, f * g).unwrap() == 24);
  assert(A::act(A::act(m, f), g).unwrap() == 24);
  assert(A::act(m * n, f).unwrap() == 16);
  assert((A::act(m, f) * A::act(n, f)).unwrap() == 16);
}

int main() {
  check_action<ScaleSum>();
  check_action<GenericScaleSum>();
}
