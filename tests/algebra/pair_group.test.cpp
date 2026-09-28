// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include "algebra/pair_group.hpp"

#include <cassert>
#include <concepts>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "algebra/group_impl.hpp"
#include "algebra/pair_monoid.hpp"
#include "data_structure/segment_tree.hpp"

using Sum = GroupImpl<int,
                      [](int a, int b) { return a + b; },
                      []() { return 0; },
                      [](int a) { return -a; }>;
// Permutations of {0, 1, 2}, with a * b applying b first and then a.
using Permutation =
    GroupImpl<std::vector<int>,
              [](const std::vector<int>& a, const std::vector<int>& b) {
                return std::vector<int>{a[b[0]], a[b[1]], a[b[2]]};
              },
              []() { return std::vector<int>{0, 1, 2}; },
              [](const std::vector<int>& a) {
                std::vector<int> inverse(3);
                for (int i = 0; i < 3; ++i) inverse[a[i]] = i;
                return inverse;
              }>;
using P = PairGroup<Permutation, Sum>;
using PermPair = PairGroup<Permutation, Permutation>;
using M = PairMonoid<Permutation, Sum>;

template <typename G, typename H>
concept HasPairGroup = requires { typename PairGroup<G, H>; };

template <typename T>
concept HasPretty = requires(const T& x) {
  { pretty(x) } -> std::same_as<std::string>;
};

struct Opaque {
  static Opaque identity() { return {}; }
  Opaque inverse() const { return {}; }
  friend Opaque operator*(const Opaque&, const Opaque&) { return {}; }
};

struct MoveOnly {
  std::unique_ptr<int> value;
  explicit MoveOnly(int x): value{std::make_unique<int>(x)} {}
  static MoveOnly identity() { return MoveOnly{0}; }
  MoveOnly inverse() const { return MoveOnly{-*value}; }
  friend MoveOnly operator*(const MoveOnly& a, const MoveOnly& b) {
    return MoveOnly{*a.value + *b.value};
  }
  friend std::string pretty(const MoveOnly& x) {
    return std::to_string(*x.value);
  }
};

// A reference element must retain its lvalue category during conversion.
struct LvalueInt {
  int value;
  operator int() const& { return value; }
  operator int() && = delete;
};

static_assert(Group<P> && Monoid<P> && std::copyable<P>);
static_assert(Monoid<M> && !Group<M> && !std::same_as<P, M>);
static_assert(HasPairGroup<Permutation, Sum>);
static_assert(!HasPairGroup<M, Sum> && !HasPairGroup<Sum, M>);
static_assert(!HasPairGroup<int, Sum> && !HasPairGroup<Sum, int>);
static_assert(std::constructible_from<P, std::vector<int>&, long long>);
static_assert(!std::constructible_from<P, int, int>);
static_assert(!std::constructible_from<P, std::vector<int>, std::string>);
static_assert(
    std::constructible_from<P, std::pair<std::vector<int>, long long>&>);
static_assert(
    std::constructible_from<P, const std::pair<std::vector<int>, long long>&>);
static_assert(
    std::constructible_from<P, std::pair<std::vector<int>, long long>&&>);
static_assert(!std::constructible_from<P, const std::pair<int, int>&>);
static_assert(!std::constructible_from<P, std::pair<int, int>&&>);
static_assert(
    !std::constructible_from<P,
                             const std::pair<std::vector<int>, std::string>&>);
static_assert(
    !std::constructible_from<P, std::pair<std::vector<int>, std::string>&&>);
static_assert(std::same_as<decltype((std::declval<P&>().first)), Permutation&>);
static_assert(std::same_as<decltype((std::declval<const P&>().first)),
                           const Permutation&>);
static_assert(std::same_as<decltype((std::declval<P&>().second)), Sum&>);
static_assert(
    std::same_as<decltype((std::declval<const P&>().second)), const Sum&>);
static_assert(HasPretty<P>);
static_assert(!HasPretty<PairGroup<Opaque, Sum>>);
static_assert(!HasPretty<PairGroup<Sum, Opaque>>);
static_assert(!std::default_initializable<MoveOnly>);
static_assert(Group<PairGroup<MoveOnly, MoveOnly>>);
static_assert(!std::copy_constructible<PairGroup<MoveOnly, MoveOnly>>);
static_assert(HasPretty<PairGroup<MoveOnly, MoveOnly>>);

void check_construction() {
  std::vector<int> left{1, 2, 0}, right{2, 0, 1};
  const std::vector<int> fixed{1, 0, 2};
  const PermPair copied{left, right};
  assert((left == std::vector<int>{1, 2, 0}));
  assert((right == std::vector<int>{2, 0, 1}));
  assert(copied.first.unwrap() == left && copied.second.unwrap() == right);

  const PermPair mixed_left{left, std::vector<int>{0, 2, 1}};
  const PermPair mixed_right{std::vector<int>{0, 2, 1}, right};
  assert((left == std::vector<int>{1, 2, 0}));
  assert((right == std::vector<int>{2, 0, 1}));
  assert(pretty(mixed_left) == "([1, 2, 0], [0, 2, 1])");
  assert(pretty(mixed_right) == "([0, 2, 1], [2, 0, 1])");
  const PermPair const_left{fixed, right}, const_right{left, fixed};
  assert(const_left.first.unwrap() == fixed);
  assert(const_right.second.unwrap() == fixed);

  // Parenthesized initialization permits the same conversions as the
  // constraint.
  const P converted{left, 3LL};
  assert(converted.first.unwrap() == left);
  assert(converted.second.unwrap() == 3);

  using Numbers = PairGroup<Sum, Sum>;
  const PairGroup<Numbers, Numbers> nested{{1, 2}, {3, 4}};
  assert(pretty(nested) == "((1, 2), (3, 4))");
  assert(pretty(nested.inverse()) == "((-1, -2), (-3, -4))");
  const PairGroup<Numbers, Sum> default_left{{1, 2}, 3LL};
  const PairGroup<Sum, Numbers> default_right{4LL, {5, 6}};
  assert(pretty(default_left) == "((1, 2), 3)");
  assert(pretty(default_right) == "(4, (5, 6))");
}

void check_pair_construction() {
  std::pair<std::vector<int>, long long> source{{1, 2, 0}, 3};
  const P copied{source}, const_copied{std::as_const(source)};
  assert((copied.first.unwrap() == std::vector<int>{1, 2, 0}));
  assert(copied.second.unwrap() == 3);
  assert(const_copied.first.unwrap() == copied.first.unwrap());
  assert(const_copied.second.unwrap() == 3);
  assert(source.first == copied.first.unwrap() && source.second == 3);

  const P converted{std::pair{std::vector<int>{1, 0, 2}, 4LL}};
  assert((converted.first.unwrap() == std::vector<int>{1, 0, 2}));
  assert(converted.second.unwrap() == 4);

  LvalueInt left{5}, right{7};
  using Numbers = PairGroup<Sum, Sum>;
  const std::pair<LvalueInt&, LvalueInt&> refs{left, right};
  const Numbers copied_refs{refs};
  const Numbers forwarded_refs{std::pair<LvalueInt&, LvalueInt&>{left, right}};
  assert(copied_refs.first.unwrap() == 5 && copied_refs.second.unwrap() == 7);
  assert(forwarded_refs.first.unwrap() == 5 &&
         forwarded_refs.second.unwrap() == 7);
  assert(left.value == 5 && right.value == 7);
}

void check_pair_move_only() {
  using Q = PairGroup<MoveOnly, MoveOnly>;
  using Source = std::pair<MoveOnly, MoveOnly>;
  static_assert(std::constructible_from<Q, Source&&>);
  static_assert(!std::constructible_from<Q, Source&>);
  static_assert(!std::constructible_from<Q, const Source&>);
  static_assert(!std::constructible_from<Q, const Source&&>);

  Source source{MoveOnly{2}, MoveOnly{3}};
  const Q moved{std::move(source)};
  assert(*moved.first.value == 2 && *moved.second.value == 3);
  assert(!source.first.value && !source.second.value);

  // Rvalue-reference members must also be forwarded as rvalues.
  MoveOnly left{5}, right{7};
  std::pair<MoveOnly&&, MoveOnly&&> refs{std::move(left), std::move(right)};
  const Q moved_refs{std::move(refs)};
  assert(*moved_refs.first.value == 5 && *moved_refs.second.value == 7);
  assert(!left.value && !right.value);
}

void check_noncommutative_operations() {
  const PermPair a{std::vector<int>{1, 0, 2}, std::vector<int>{0, 2, 1}};
  const PermPair b{std::vector<int>{0, 2, 1}, std::vector<int>{1, 0, 2}};
  const PermPair e = PermPair::identity();
  assert(pretty(a * b) == "([1, 2, 0], [2, 0, 1])");
  assert(pretty(b * a) == "([2, 0, 1], [1, 2, 0])");
  const auto equal = [](const PermPair& x, const PermPair& y) {
    return x.first.unwrap() == y.first.unwrap() &&
           x.second.unwrap() == y.second.unwrap();
  };
  const std::vector<PermPair> values{e, a, b, a * b};
  for (const PermPair& x: values) {
    assert(equal(x * e, x) && equal(e * x, x));
    assert(equal(x * x.inverse(), e) && equal(x.inverse() * x, e));
    assert(equal(x.inverse().inverse(), x));
    for (const PermPair& y: values) {
      assert(equal((x * y).inverse(), y.inverse() * x.inverse()));
      for (const PermPair& z: values) {
        assert(equal((x * y) * z, x * (y * z)));
      }
    }
  }

  SegmentTree<PermPair> tree{std::vector<PermPair>{a, b}};
  assert(equal(tree.fold(), a * b));
  assert(equal(tree.fold(1, 2), b));
  assert(equal(tree.fold(1, 1), e));
  tree.set(0, e);
  assert(equal(tree.fold(), b));
  assert(equal(SegmentTree<PermPair>{0}.fold(), e));
}

void check_move_only() {
  using Q = PairGroup<MoveOnly, MoveOnly>;
  const Q a{MoveOnly{2}, MoveOnly{3}}, b{MoveOnly{5}, MoveOnly{7}};
  const Q e{};
  assert(pretty(e) == "(0, 0)");
  assert(pretty(a * b) == "(7, 10)");
  assert(pretty(a.inverse()) == "(-2, -3)");
  assert(pretty(a * a.inverse()) == "(0, 0)");
  assert(pretty(a.inverse() * a) == "(0, 0)");
  assert(pretty(a * Q::identity()) == "(2, 3)");
  assert(pretty(Q::identity() * a) == "(2, 3)");
}

int main() {
  check_construction();
  check_pair_construction();
  check_pair_move_only();
  check_noncommutative_operations();
  check_move_only();

  const P a{std::vector<int>{1, 2, 0}, 3};
  const P b{std::vector<int>{1, 0, 2}, -2};
  assert(pretty(P{}) == "([0, 1, 2], 0)");
  assert(pretty(a * b) == "([2, 1, 0], 1)");
  assert(pretty(a.inverse()) == "([2, 0, 1], -3)");

  P copied{a};
  copied.first.unwrap() = {0, 2, 1};
  copied.second.unwrap() = 9;
  assert(pretty(std::as_const(copied)) == "([0, 2, 1], 9)");
  assert(pretty(a) == "([1, 2, 0], 3)");
  copied = b;
  assert(pretty(copied) == pretty(b));
  P moved{std::move(copied)};
  assert(pretty(moved) == pretty(b));
  moved = P::identity();
  assert(pretty(moved) == "([0, 1, 2], 0)");

  const PairGroup<P, Sum> nested{a, 5};
  assert(pretty(nested.inverse()) == "(([2, 0, 1], -3), -5)");
  assert(pretty(nested * nested.inverse()) == "(([0, 1, 2], 0), 0)");
  assert(pretty(std::vector<P>{a, b}) == "[([1, 2, 0], 3), ([1, 0, 2], -2)]");
  const PairGroup<Opaque, Sum> opaque{Opaque{}, 3};
  assert((opaque * opaque.inverse()).second.unwrap() == 0);
}
