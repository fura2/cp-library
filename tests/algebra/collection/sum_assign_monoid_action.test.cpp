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
concept ValidAddAssign = requires { typename SumAssignMonoidAction<T>; };

template <>
inline constexpr auto id_zero<long double> = []() { return 0.0L; };

static_assert(ValidAddAssign<int> && ValidAddAssign<long double>);
static_assert(!ValidAddAssign<float> && !ValidAddAssign<short>);

template <typename M, typename T>
void check_value(const M& value, const T& sum, int length) {
  assert(value.first.unwrap() == sum);
  assert(value.second.unwrap() == length);
}

template <MonoidAction A>
void check_action() {
  using M = A::value_monoid;
  using F = A::action_monoid;
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

template <MonoidAction A, typename T>
void check_ranges(const LazySegmentTree<A>& tree,
                  const std::vector<T>& values) {
  const int n = values.size();
  assert(tree.size() == n);
  T total{};
  for (const T& value: values) total += value;
  // Check the root before subrange queries push pending assignments down.
  check_value(tree.fold(), total, n);
  for (int l = 0; l <= n; ++l) {
    T sum{};
    for (int r = l; r <= n; ++r) {
      check_value(tree.fold(l, r), sum, r - l);
      if (r < n) sum += values[r];
    }
  }
  check_value(tree.fold(), total, n);
}

template <MonoidAction A>
void check_tree() {
  using M = A::value_monoid;
  using F = A::action_monoid;
  using T = std::remove_cvref_t<decltype(M{}.first.unwrap())>;
  using Tree = LazySegmentTree<A>;

  Tree empty{0};
  empty.apply(0, 0, T{9});
  check_ranges(empty, std::vector<T>{});

  // Each element has length 1, even when its value is 0.
  Tree single{std::vector<M>{M{AddMonoid<T>{T{4}}, AddMonoid<int>{1}}}};
  single.apply(0, 1, T{0});
  check_value(single.fold(), T{0}, 1);
  single.apply(0, 1, F{T{-3}});
  check_ranges(single, std::vector<T>{T{-3}});

  Tree zeros{std::vector<M>(3, M{T{0}, 1})};
  zeros.apply(0, 3, T{2});
  check_ranges(zeros, std::vector<T>{T{2}, T{2}, T{2}});

  Tree tree{std::vector<M>{
      M{T{1}, 1}, M{T{2}, 1}, M{T{3}, 1}, M{T{4}, 1}, M{T{5}, 1}}};
  check_ranges(tree, std::vector<T>{T{1}, T{2}, T{3}, T{4}, T{5}});
  // Compose overlapping assignments without querying subranges between them.
  tree.apply(0, 5, T{7});
  check_value(tree.fold(), T{35}, 5);
  tree.apply(0, 4, F{T{-2}});
  check_value(tree.fold(), T{-1}, 5);
  tree.apply(1, 5, T{0});
  check_value(tree.fold(), T{-2}, 5);
  tree.apply(2, 4, T{3});
  tree.apply(0, 5, F::identity());
  tree.apply(0, 0, T{99});
  tree.apply(3, 3, T{99});
  tree.apply(5, 5, T{99});
  check_ranges(tree, std::vector<T>{T{-2}, T{0}, T{3}, T{3}, T{0}});
}

void check_numeric_types() {
  using L = SumAssignMonoidAction<long long>;
  using D = SumAssignMonoidAction<double>;
  check_value(
      L::act(L::value_monoid{0LL, 3}, L::action_monoid{3'000'000'000LL}),
      9'000'000'000LL,
      3);
  check_value(D::act(D::value_monoid{0.0, 3}, D::action_monoid{0.25}), 0.75, 3);

  using A = SumAssignMonoidAction<mint>;
  using M = A::value_monoid;
  using F = A::action_monoid;
  static_assert(MonoidAction<A>);
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
  check_tree<SumAssignMonoidAction<int>>();
  check_tree<SumAssignMonoidAction<long long>>();
  check_tree<SumAssignMonoidAction<double>>();
  check_numeric_types();
}
