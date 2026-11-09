// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include <cassert>
#include <concepts>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "algebra/group_impl.hpp"
#include "algebra/monoid_impl.hpp"
#include "algebra/pair_group.hpp"
#include "algebra/pair_monoid.hpp"

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

template <template <typename, typename> typename Pair, typename A, typename B>
concept HasPair = requires { typename Pair<A, B>; };

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

// Both pair classes implement these constructors independently.
template <template <typename, typename> typename Pair>
void check_construction() {
  using P = Pair<Permutation, Sum>;
  using PP = Pair<Permutation, Permutation>;
  using Source = std::pair<std::vector<int>, long long>;
  static_assert(Monoid<P> && std::copyable<P> && HasPretty<P>);
  static_assert(!HasPair<Pair, int, Sum> && !HasPair<Pair, Sum, int>);
  static_assert(std::constructible_from<P, std::vector<int>&, long long>);
  static_assert(!std::constructible_from<P, int, int>);
  static_assert(!std::constructible_from<P, std::vector<int>, std::string>);
  static_assert(std::constructible_from<P, Source&> &&
                std::constructible_from<P, const Source&> &&
                std::constructible_from<P, Source&&>);
  static_assert(!std::constructible_from<P, const std::pair<int, int>&>);
  static_assert(!std::constructible_from<P, std::pair<int, int>&&>);
  using WrongSecond = std::pair<std::vector<int>, std::string>;
  static_assert(!std::constructible_from<P, const WrongSecond&> &&
                !std::constructible_from<P, WrongSecond&&>);
  static_assert(
      std::same_as<decltype((std::declval<P&>().first)), Permutation&>);
  static_assert(std::same_as<decltype((std::declval<const P&>().first)),
                             const Permutation&>);
  static_assert(std::same_as<decltype((std::declval<P&>().second)), Sum&>);
  static_assert(
      std::same_as<decltype((std::declval<const P&>().second)), const Sum&>);
  static_assert(!HasPretty<Pair<Opaque, Sum>> && !HasPretty<Pair<Sum, Opaque>>);

  std::vector<int> left{1, 2, 0}, right{2, 0, 1};
  const std::vector<int> fixed{1, 0, 2};
  const PP copied{left, right};
  const PP mixed_left{left, std::vector<int>{0, 2, 1}};
  const PP mixed_right{std::vector<int>{0, 2, 1}, right};
  const PP const_left{fixed, right}, const_right{left, fixed};
  assert((left == std::vector<int>{1, 2, 0}));
  assert((right == std::vector<int>{2, 0, 1}));
  assert(copied.first.unwrap() == left && copied.second.unwrap() == right);
  assert(pretty(mixed_left) == "([1, 2, 0], [0, 2, 1])");
  assert(pretty(mixed_right) == "([0, 2, 1], [2, 0, 1])");
  assert(const_left.first.unwrap() == fixed);
  assert(const_right.second.unwrap() == fixed);

  // Parenthesized initialization must permit the constraint's conversions.
  const P converted{left, 3LL};
  assert(converted.first.unwrap() == left && converted.second.unwrap() == 3);
  using Numbers = Pair<Sum, Sum>;
  const Pair<Numbers, Numbers> nested{{1, 2}, {3, 4}};
  const Pair<Numbers, Sum> default_left{{1, 2}, 3LL};
  const Pair<Sum, Numbers> default_right{4LL, {5, 6}};
  assert(pretty(nested) == "((1, 2), (3, 4))");
  assert(pretty(default_left) == "((1, 2), 3)");
  assert(pretty(default_right) == "(4, (5, 6))");

  Source source{{1, 2, 0}, 3};
  const P from_pair{source}, from_const_pair{std::as_const(source)};
  const P from_temporary{Source{{1, 0, 2}, 4LL}};
  assert(pretty(from_pair) == "([1, 2, 0], 3)");
  assert(pretty(from_const_pair) == "([1, 2, 0], 3)");
  assert(pretty(from_temporary) == "([1, 0, 2], 4)");
  assert(source.first == left && source.second == 3);

  LvalueInt x{5}, y{7};
  const std::pair<LvalueInt&, LvalueInt&> refs{x, y};
  const Numbers copied_refs{refs};
  const Numbers forwarded_refs{std::pair<LvalueInt&, LvalueInt&>{x, y}};
  assert(pretty(copied_refs) == "(5, 7)");
  assert(pretty(forwarded_refs) == "(5, 7)");
  assert(x.value == 5 && y.value == 7);
}

template <template <typename, typename> typename Pair>
void check_move_only() {
  using Q = Pair<MoveOnly, MoveOnly>;
  using Source = std::pair<MoveOnly, MoveOnly>;
  static_assert(!std::default_initializable<MoveOnly>);
  static_assert(Monoid<Q> && !std::copy_constructible<Q> && HasPretty<Q>);
  static_assert(std::constructible_from<Q, Source&&>);
  static_assert(!std::constructible_from<Q, Source&>);
  static_assert(!std::constructible_from<Q, const Source&>);
  static_assert(!std::constructible_from<Q, const Source&&>);

  Source source{MoveOnly{2}, MoveOnly{3}};
  const Q moved{std::move(source)};
  assert(pretty(moved) == "(2, 3)");
  assert(!source.first.value && !source.second.value);
  // Rvalue-reference members must also be forwarded as rvalues.
  MoveOnly left{5}, right{7};
  std::pair<MoveOnly&&, MoveOnly&&> refs{std::move(left), std::move(right)};
  const Q moved_refs{std::move(refs)};
  assert(pretty(moved_refs) == "(5, 7)");
  assert(!left.value && !right.value);

  const Q a{MoveOnly{2}, MoveOnly{3}}, b{MoveOnly{5}, MoveOnly{7}};
  assert(pretty(Q{}) == "(0, 0)");
  assert(pretty(a * b) == "(7, 10)");
  assert(pretty(a * Q::identity()) == "(2, 3)");
  assert(pretty(Q::identity() * a) == "(2, 3)");
  if constexpr (Group<Q>) {
    assert(pretty(a.inverse()) == "(-2, -3)");
    assert(pretty(a * a.inverse()) == "(0, 0)");
  }
}

template <template <typename, typename> typename Pair>
void check_operations() {
  using PP = Pair<Permutation, Permutation>;
  const PP a{std::vector<int>{1, 0, 2}, std::vector<int>{0, 2, 1}};
  const PP b{std::vector<int>{0, 2, 1}, std::vector<int>{1, 0, 2}};
  const PP e = PP::identity();
  assert(pretty(a * b) == "([1, 2, 0], [2, 0, 1])");
  assert(pretty(b * a) == "([2, 0, 1], [1, 2, 0])");
  const auto equal = [](const PP& x, const PP& y) {
    return x.first.unwrap() == y.first.unwrap() &&
           x.second.unwrap() == y.second.unwrap();
  };
  for (const PP& x: {e, a, b, a * b}) {
    assert(equal(x * e, x) && equal(e * x, x));
    if constexpr (Group<PP>) {
      assert(equal(x * x.inverse(), e) && equal(x.inverse() * x, e));
      assert(equal(x.inverse().inverse(), x));
    }
  }
  if constexpr (Group<PP>) {
    assert(equal((a * b).inverse(), b.inverse() * a.inverse()));
  }

  using P = Pair<Permutation, Sum>;
  const P p{std::vector<int>{1, 2, 0}, 3}, q{std::vector<int>{1, 0, 2}, -2};
  assert(pretty(P{}) == "([0, 1, 2], 0)");
  assert(pretty(p * q) == "([2, 1, 0], 1)");
  P copied{p};
  copied.first.unwrap() = {0, 2, 1};
  copied.second.unwrap() = 9;
  assert(pretty(std::as_const(copied)) == "([0, 2, 1], 9)");
  assert(pretty(p) == "([1, 2, 0], 3)");
  const Pair<P, Sum> nested{p, 5};
  assert(pretty(nested) == "(([1, 2, 0], 3), 5)");
  assert(pretty(nested * decltype(nested)::identity()) == pretty(nested));
  if constexpr (Group<P>) {
    assert(pretty(p.inverse()) == "([2, 0, 1], -3)");
    assert(pretty(nested.inverse()) == "(([2, 0, 1], -3), -5)");
    assert(pretty(nested * nested.inverse()) == "(([0, 1, 2], 0), 0)");
  }
  assert(pretty(std::vector<P>{p, q}) == "[([1, 2, 0], 3), ([1, 0, 2], -2)]");
  const Pair<Opaque, Sum> opaque{Opaque{}, 3};
  assert((opaque * opaque).second.unwrap() == 6);
  if constexpr (Group<P>) {
    assert((opaque * opaque.inverse()).second.unwrap() == 0);
  }
}

int main() {
  using M = PairMonoid<Permutation, Sum>;
  using G = PairGroup<Permutation, Sum>;
  static_assert(Monoid<M> && !Group<M> && Group<G> && !std::same_as<M, G>);
  static_assert(Group<PairGroup<MoveOnly, MoveOnly>>);
  static_assert(!HasPair<PairGroup, M, Sum> && !HasPair<PairGroup, Sum, M>);
  check_construction<PairMonoid>();
  check_construction<PairGroup>();
  check_move_only<PairMonoid>();
  check_move_only<PairGroup>();
  check_operations<PairMonoid>();
  check_operations<PairGroup>();

  using Concat = MonoidImpl<std::string,
                            [](const std::string& a, const std::string& b) {
                              return a + b;
                            },
                            []() { return std::string{}; }>;
  static_assert(HasPair<PairMonoid, Concat, Sum>);
  static_assert(!HasPair<PairGroup, Concat, Sum>);
  using Text = PairMonoid<Concat, Sum>;
  const Text text{"ab", 3LL};
  assert(pretty(text) == "(\"ab\", 3)");
  assert(pretty(text * Text{"cd", -1}) == "(\"abcd\", 2)");
  assert(pretty(text * Text::identity()) == pretty(text));
}
