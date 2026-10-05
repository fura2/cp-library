#include "algebra/collection/sum_assign_monoid_action.hpp"

#include <cassert>
#include <concepts>
#include <optional>
#include <type_traits>
#include <vector>

#include "algebra/collection/mint_add_monoid.hpp"
#include "algebra/monoid_action.hpp"
#include "data_structure/lazy_segment_tree.hpp"

template <typename T>
concept ValidSumAssign = requires { typename SumAssignMonoidAction<T>; };

template <>
inline constexpr auto id_zero<long double> = []() { return 0.0L; };

static_assert(ValidSumAssign<int> && ValidSumAssign<long double>);
static_assert(!ValidSumAssign<float> && !ValidSumAssign<short>);

template <typename M, typename T>
void check_value(const M& value, const T& sum, int length) {
  assert(value.first.unwrap() == sum);
  assert(value.second.unwrap() == length);
}

template <MonoidActionOnMonoid A>
void check_action() {
  using M = A::value_type;
  using F = A::action_type;
  using T = std::remove_cvref_t<decltype(M{}.first.unwrap())>;
  static_assert(std::same_as<M, PairMonoid<AddMonoid<T>, AddMonoid<int>>>);
  static_assert(std::same_as<F, LastMonoid<T>>);
  static_assert(std::same_as<decltype(M{}.second.unwrap()), int&>);

  const M m{T{7}, 3}, n{T{-2}, 2};
  const F assign_four{T{4}}, assign_minus_one{T{-1}}, assign_zero{T{0}};
  check_value(A::act(m, F{}), T{7}, 3);
  check_value(A::act(m, F{std::nullopt}), T{7}, 3);
  check_value(A::act(m, assign_four), T{12}, 3);
  check_value(A::act(m, assign_zero), T{0}, 3);
  check_value(A::act(m, assign_four * assign_minus_one), T{-3}, 3);
  check_value(A::act(m, assign_minus_one * assign_four), T{12}, 3);

  const auto check_equal = [](const M& a, const M& b) {
    check_value(a, b.first.unwrap(), b.second.unwrap());
  };
  const std::vector<F> actions{
      F::identity(), assign_four, assign_minus_one, assign_zero};
  for (const F& f: actions) {
    check_equal(A::act(M::identity(), f), M::identity());
    check_equal(A::act(m * n, f), A::act(m, f) * A::act(n, f));
    for (const F& g: actions) {
      check_equal(A::act(m, f * g), A::act(A::act(m, f), g));
    }
  }
  check_value(m, T{7}, 3);
  assert(assign_four.unwrap() == T{4});
  const M wrapped{AddMonoid<T>{T{7}}, AddMonoid<int>{3}};
  check_value(A::act(wrapped, assign_four), T{12}, 3);
}

void check_tree() {
  using A = SumAssignMonoidAction<int>;
  // Keep one usage example; range boundaries are covered by the tree's tests.
  // Each element has length 1, even when its value is 0.
  LazySegmentTree<A> tree{std::vector<A::value_type>{{1, 1}, {0, 1}, {3, 1}}};
  tree.apply(0, 3, 4);
  tree.apply(1, 3, 0);
  tree.apply(0, 3, A::action_type::identity());
  check_value(tree.fold(), 4, 3);
  check_value(tree.fold(1, 3), 0, 2);
}

void check_numeric_types() {
  using L = SumAssignMonoidAction<long long>;
  using D = SumAssignMonoidAction<double>;
  check_value(L::act(L::value_type{0LL, 3}, L::action_type{3'000'000'000LL}),
              9'000'000'000LL,
              3);
  check_value(D::act(D::value_type{0.0, 3}, D::action_type{0.25}), 0.75, 3);

  using A = SumAssignMonoidAction<mint>;
  using M = A::value_type;
  using F = A::action_type;
  static_assert(MonoidActionOnMonoid<A>);
  static_assert(std::same_as<M, PairMonoid<MintAddMonoid, AddMonoid<int>>>);
  const M value{MintAddMonoid{mint{7}}, AddMonoid<int>{3}};
  const auto assigned = A::act(value, F{mint{-1}});
  assert(assigned.first.unwrap().unwrap() == mint{-3}.unwrap());
  assert(assigned.second.unwrap() == 3);
  assert(A::act(value, F::identity()).first.unwrap().unwrap() == 7);
  assert(A::act(M::identity(), F{mint{5}}).first.unwrap().unwrap() == 0);
}

int main() {
  check_action<SumAssignMonoidAction<int>>();
  check_action<SumAssignMonoidAction<long long>>();
  check_action<SumAssignMonoidAction<double>>();
  check_action<SumAssignMonoidAction<long double>>();
  check_tree();
  check_numeric_types();
}
