// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include "algebra/pair_monoid.hpp"

#include <cassert>
#include <concepts>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "algebra/monoid_impl.hpp"
#include "data_structure/segment_tree.hpp"

using Concat =
    MonoidImpl<std::string,
               [](const std::string& a, const std::string& b) { return a + b; },
               []() { return std::string{}; }>;
using Sum =
    MonoidImpl<int, [](int a, int b) { return a + b; }, []() { return 0; }>;
using P = PairMonoid<Concat, Sum>;
using TextPair = PairMonoid<Concat, Concat>;

template <typename M, typename N>
concept HasPairMonoid = requires { typename PairMonoid<M, N>; };

template <typename T>
concept HasPretty = requires(const T& x) {
  { pretty(x) } -> std::same_as<std::string>;
};

struct Opaque {
  static Opaque identity() { return {}; }
  friend Opaque operator*(const Opaque&, const Opaque&) { return {}; }
};

struct MoveOnly {
  std::unique_ptr<int> value;
  explicit MoveOnly(int x): value{std::make_unique<int>(x)} {}
  static MoveOnly identity() { return MoveOnly{0}; }
  friend MoveOnly operator*(const MoveOnly& a, const MoveOnly& b) {
    return MoveOnly{*a.value + *b.value};
  }
  friend std::string pretty(const MoveOnly& x) {
    return std::to_string(*x.value);
  }
};

static_assert(Monoid<P> && std::copyable<P>);
static_assert(HasPairMonoid<Concat, Sum>);
static_assert(!HasPairMonoid<int, Sum> && !HasPairMonoid<Sum, int>);
static_assert(std::constructible_from<P, const char*, long long>);
static_assert(!std::constructible_from<P, int, int>);
static_assert(!std::constructible_from<P, std::string, std::string>);
static_assert(std::same_as<decltype((std::declval<P&>().first)), Concat&>);
static_assert(
    std::same_as<decltype((std::declval<const P&>().first)), const Concat&>);
static_assert(std::same_as<decltype((std::declval<P&>().second)), Sum&>);
static_assert(
    std::same_as<decltype((std::declval<const P&>().second)), const Sum&>);
static_assert(HasPretty<P>);
static_assert(!HasPretty<PairMonoid<Opaque, Sum>>);
static_assert(!HasPretty<PairMonoid<Sum, Opaque>>);
static_assert(!std::default_initializable<MoveOnly>);
static_assert(Monoid<PairMonoid<MoveOnly, MoveOnly>>);
static_assert(!std::copy_constructible<PairMonoid<MoveOnly, MoveOnly>>);
static_assert(HasPretty<PairMonoid<MoveOnly, MoveOnly>>);

void check_construction() {
  std::string left = "left", right = "right";
  const std::string fixed = "fixed";
  const TextPair copied{left, right};
  assert(left == "left" && right == "right");
  assert(copied.first.unwrap() == "left");
  assert(copied.second.unwrap() == "right");

  const TextPair mixed_left{left, std::string{"temporary"}};
  const TextPair mixed_right{std::string{"temporary"}, right};
  assert(left == "left" && right == "right");
  assert(mixed_left.first.unwrap() == "left");
  assert(mixed_left.second.unwrap() == "temporary");
  assert(mixed_right.first.unwrap() == "temporary");
  assert(mixed_right.second.unwrap() == "right");
  const TextPair const_left{fixed, right}, const_right{left, fixed};
  assert(const_left.first.unwrap() == "fixed");
  assert(const_right.second.unwrap() == "fixed");

  // Construction uses the same parenthesized initialization as the constraint.
  const P converted{"text", 3LL};
  const PairMonoid<Sum, Sum> numbers{4LL, 5LL};
  assert(converted.first.unwrap() == "text");
  assert(converted.second.unwrap() == 3);
  assert(numbers.first.unwrap() == 4 && numbers.second.unwrap() == 5);
}

void check_noncommutative_operations() {
  const TextPair a{"ab", "XY"}, b{"cd", "ZW"};
  assert(pretty(a * b) == "(\"abcd\", \"XYZW\")");
  assert(pretty(b * a) == "(\"cdab\", \"ZWXY\")");
  const std::vector<TextPair> values{TextPair::identity(), a, b};
  for (const TextPair& x: values) {
    for (const TextPair& y: values) {
      for (const TextPair& z: values) {
        const TextPair left = (x * y) * z, right = x * (y * z);
        assert(left.first.unwrap() == right.first.unwrap());
        assert(left.second.unwrap() == right.second.unwrap());
      }
    }
  }
}

void check_move_only() {
  using Q = PairMonoid<MoveOnly, MoveOnly>;
  const Q a{MoveOnly{2}, MoveOnly{3}}, b{MoveOnly{5}, MoveOnly{7}};
  const Q e{};
  assert(*e.first.value == 0 && *e.second.value == 0);
  const Q product = a * b;
  assert(*product.first.value == 7 && *product.second.value == 10);
  assert(pretty(product) == "(7, 10)");
  assert(pretty(e) == "(0, 0)");
  assert(pretty(a * Q::identity()) == "(2, 3)");
  assert(pretty(Q::identity() * a) == "(2, 3)");
}

int main() {
  check_construction();
  check_noncommutative_operations();
  check_move_only();

  const P a{"ab", 2}, b{"cd", 3}, e = P::identity();
  assert(e.first.unwrap().empty() && e.second.unwrap() == 0);
  assert(pretty(P{}) == "(\"\", 0)");
  assert(pretty(a * e) == pretty(a) && pretty(e * a) == pretty(a));
  const P product = a * b;
  assert(product.first.unwrap() == "abcd" && product.second.unwrap() == 5);

  P copied{a};
  copied.first.unwrap() = "changed";
  copied.second.unwrap() = 9;
  assert(std::as_const(copied).first.unwrap() == "changed");
  assert(std::as_const(copied).second.unwrap() == 9);
  assert(a.first.unwrap() == "ab" && a.second.unwrap() == 2);
  copied = b;
  assert(pretty(copied) == pretty(b));
  P moved{std::move(copied)};
  assert(pretty(moved) == pretty(b));
  moved = P::identity();
  assert(pretty(moved) == pretty(e));

  using Nested = PairMonoid<P, Sum>;
  const Nested nested{a, 7};
  assert(pretty(nested) == "((\"ab\", 2), 7)");
  assert(pretty(nested * Nested::identity()) == pretty(nested));
  assert(pretty(std::vector<P>{e, a}) == "[(\"\", 0), (\"ab\", 2)]");
  const PairMonoid<Opaque, Sum> opaque{};
  assert((opaque * opaque).second.unwrap() == 0);

  SegmentTree<P> tree(std::vector<P>{a, b});
  assert(pretty(tree.fold()) == "(\"abcd\", 5)");
  assert(pretty(tree.fold(1, 2)) == pretty(b));
  assert(pretty(tree.fold(1, 1)) == pretty(e));
  tree.set(0, P{"x", 7});
  assert(pretty(tree.fold()) == "(\"xcd\", 10)");
  assert(pretty(SegmentTree<P>{0}.fold()) == pretty(e));
}
