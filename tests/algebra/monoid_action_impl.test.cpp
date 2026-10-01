#include "algebra/monoid_action_impl.hpp"

#include <cassert>
#include <concepts>
#include <string>

#include "algebra/collection/add_group.hpp"
#include "algebra/collection/last_monoid.hpp"
#include "algebra/collection/mul_monoid.hpp"
#include "algebra/monoid_action.hpp"
#include "algebra/semigroup.hpp"

using M = AddGroup<int>;
using F = MulMonoid<int>;

M scale(const M& m, const F& f) { return M{m.unwrap() * f.unwrap()}; }

using ScaleSum = MonoidActionImpl<M, F, scale>;
using GenericScaleSum =
    MonoidActionImpl<M, F, [](const auto& m, const auto& f) {
      return M{m.unwrap() * f.unwrap()};
    }>;

template <typename V, typename U, auto Act>
concept ValidActionImpl = requires { typename MonoidActionImpl<V, U, Act>; };

static_assert(MonoidAction<ScaleSum> && MonoidAction<GenericScaleSum>);
static_assert(MonoidActionOnMonoid<ScaleSum> &&
              MonoidActionOnMonoid<GenericScaleSum>);
static_assert(std::same_as<ScaleSum::value_type, M>);
static_assert(std::same_as<ScaleSum::action_type, F>);
static_assert(!ValidActionImpl<int, F, scale>);
static_assert(!ValidActionImpl<M, int, scale>);
static_assert(!ValidActionImpl<M, F, [](const M&, const F&) { return 0; }>);
static_assert(
    !ValidActionImpl<M, F, [](const M& m, const F&) -> const M& { return m; }>);
static_assert(!ValidActionImpl<M, F, [](M& m, F&) { return m; }>);

template <MonoidActionOnMonoid A>
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

void check_string_assignment() {
  using Assign = LastMonoid<std::string>;
  using A = MonoidActionImpl<std::string,
                             Assign,
                             [](const std::string& x, const Assign& f) {
                               return f.unwrap().value_or(x);
                             }>;
  static_assert(!Semigroup<std::string>);
  static_assert(MonoidAction<A> && !MonoidActionOnMonoid<A>);
  static_assert(std::same_as<A::value_type, std::string>);
  static_assert(std::same_as<A::action_type, Assign>);

  const std::string x = "initial";
  const Assign f{std::string{"first"}}, g{std::string{"second"}};
  assert(A::act(x, Assign::identity()) == x);
  assert(A::act(x, f) == "first");
  assert(A::act(x, f * g) == "second");
  assert(A::act(x, f * g) == A::act(A::act(x, f), g));
  assert(A::act(x, g * f) == "first");
  assert(A::act(x, Assign{std::string{}}).empty());
}

int main() {
  check_action<ScaleSum>();
  check_action<GenericScaleSum>();
  check_string_assignment();
}
