#include "algebra/collection/max_add_monoid_action.hpp"

#include <cassert>
#include <concepts>
#include <vector>

#include "algebra/monoid_action.hpp"
#include "data_structure/lazy_segment_tree.hpp"

template <typename T>
concept ValidMaxAdd = requires { typename MaxAddMonoidAction<T>; };

static_assert(ValidMaxAdd<int> && ValidMaxAdd<long long> &&
              ValidMaxAdd<double>);
static_assert(!ValidMaxAdd<float> && !ValidMaxAdd<short>);
static_assert(MonoidActionOnMonoid<MaxAddMonoidAction<int>>);
static_assert(MonoidActionOnMonoid<MaxAddMonoidAction<long long>>);
static_assert(MonoidActionOnMonoid<MaxAddMonoidAction<double>>);

void check_action() {
  using A = MaxAddMonoidAction<int>;
  using M = A::value_type;
  using F = A::action_type;
  static_assert(std::same_as<M, MaxMonoid<int>>);
  static_assert(std::same_as<F, AddMonoid<int>>);

  const M m{7}, n{-2}, e = M::identity();
  assert(A::act(m, F{4}).unwrap() == 11);
  assert(A::act(n, F{-3}).unwrap() == -5);
  // The sentinel is special only for values, not for additive actions.
  assert(A::act(m, F{-INF}).unwrap() == 7 - INF);

  const std::vector<M> values{e, m, n};
  const std::vector<F> actions{F::identity(), F{-3}, F{4}};
  for (const M& x: values) {
    assert(A::act(x, F::identity()).unwrap() == x.unwrap());
  }
  for (const F& f: actions) {
    // -INF is finite in storage, but must remain the identity after addition.
    assert(A::act(e, f).unwrap() == e.unwrap());
    for (const M& x: values) {
      assert(A::act(x * n, f).unwrap() ==
             (A::act(x, f) * A::act(n, f)).unwrap());
      for (const F& g: actions) {
        assert(A::act(x, f * g).unwrap() == A::act(A::act(x, f), g).unwrap());
      }
    }
  }
}

void check_tree() {
  using A = MaxAddMonoidAction<int>;
  using M = A::value_type;
  const int identity = M::identity().unwrap();

  // Adding to -INF used to make fold() disagree with fold(0, 1).
  LazySegmentTree<A> single{1};
  single.apply(0, 1, -1);
  assert(single.fold().unwrap() == identity);
  assert(single.fold(0, 1).unwrap() == identity);
  assert(single.get(0).unwrap() == identity);

  // Keep one mixed-value example; general range boundaries are tested
  // elsewhere.
  LazySegmentTree<A> tree{std::vector<M>{M{3}, M::identity(), M{-2}}};
  tree.apply(0, 3, 4);
  tree.apply(1, 3, -3);
  assert(tree.fold().unwrap() == 7);
  assert(tree.fold(1, 3).unwrap() == -1);
  assert(tree.fold(1, 2).unwrap() == identity);
  assert(tree.get(1).unwrap() == identity);
}

void check_numeric_types() {
  using L = MaxAddMonoidAction<long long>;
  assert(
      L::act(L::value_type{3'000'000'000LL}, L::action_type{-7'000'000'000LL})
          .unwrap() == -4'000'000'000LL);
  assert(L::act(L::value_type::identity(), L::action_type{1LL}).unwrap() ==
         -LINF);
  assert(L::act(L::value_type::identity(), L::action_type{-1LL}).unwrap() ==
         -LINF);

  using D = MaxAddMonoidAction<double>;
  assert(D::act(D::value_type{1.5}, D::action_type{-0.25}).unwrap() == 1.25);
  // Small increments would round back to -DINF even without the identity guard.
  assert(D::act(D::value_type::identity(), D::action_type{1e99}).unwrap() ==
         -DINF);
  assert(D::act(D::value_type::identity(), D::action_type{-1e99}).unwrap() ==
         -DINF);
}

int main() {
  check_action();
  check_tree();
  check_numeric_types();
}
