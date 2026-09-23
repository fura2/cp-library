// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include "algebra/unitization_of_semigroup.hpp"

#include <cassert>
#include <concepts>
#include <string>
#include <utility>
#include <vector>

#include "algebra/monoid.hpp"
#include "algebra/semigroup_impl.hpp"
#include "data_structure/foldable_queue.hpp"
#include "data_structure/segment_tree.hpp"
#include "mint/mint.hpp"

using Concat =
    SemigroupImpl<std::string, [](const std::string& a, const std::string& b) {
      return a + b;
    }>;
using M = UnitizationOfSemigroup<Concat>;

template <typename T>
concept HasUnwrap = requires(T& x) { x.unwrap(); };

template <typename T>
concept HasPretty = requires(const T& x) {
  { pretty(x) } -> std::same_as<std::string>;
};

struct MutableOnly {
  int x;
  int& unwrap() { return x; }
  friend MutableOnly operator*(MutableOnly a, MutableOnly b) {
    return {a.x + b.x};
  }
};

struct ConstOnly {
  int x;
  const int& unwrap() const { return x; }
  friend ConstOnly operator*(ConstOnly a, ConstOnly b) { return {a.x + b.x}; }
};

struct Opaque {
  friend Opaque operator*(const Opaque&, const Opaque&) { return {}; }
};

static_assert(Semigroup<Concat> && !Monoid<Concat> && Monoid<M>);
static_assert(!std::default_initializable<Concat> &&
              std::default_initializable<M>);
static_assert(std::copyable<M>);
static_assert(
    std::same_as<decltype(std::declval<M&>().unwrap()), std::string&>);
static_assert(std::same_as<decltype(std::declval<const M&>().unwrap()),
                           const std::string&>);
static_assert(HasUnwrap<UnitizationOfSemigroup<MutableOnly>>);
static_assert(!HasUnwrap<const UnitizationOfSemigroup<MutableOnly>>);
static_assert(!HasPretty<UnitizationOfSemigroup<MutableOnly>>);
static_assert(!HasPretty<UnitizationOfSemigroup<ConstOnly>>);
static_assert(
    std::same_as<
        decltype(std::declval<UnitizationOfSemigroup<ConstOnly>&>().unwrap()),
        const int&>);
static_assert(std::same_as<
              decltype(std::declval<UnitizationOfSemigroup<mint>&>().unwrap()),
              int>);
static_assert(
    std::same_as<
        decltype(std::declval<const UnitizationOfSemigroup<mint>&>().unwrap()),
        int>);
static_assert(HasPretty<UnitizationOfSemigroup<int>>);
static_assert(!HasUnwrap<UnitizationOfSemigroup<int>>);
static_assert(Monoid<UnitizationOfSemigroup<Opaque>>);
static_assert(!HasUnwrap<UnitizationOfSemigroup<Opaque>>);
static_assert(!HasPretty<UnitizationOfSemigroup<Opaque>>);

void check_data_structures() {
  SegmentTree<M> tree(
      std::vector<Concat>{Concat{"a"}, Concat{"b"}, Concat{"c"}});
  assert(tree.fold().unwrap() == "abc");
  assert(tree.fold(1, 3).unwrap() == "bc");
  assert(tree.fold(1, 1).is_identity());
  const auto at_most_two = [](const M& x) {
    return x.is_identity() || x.unwrap().size() <= 2;
  };
  assert(tree.max_right(0, at_most_two) == 2);
  assert(tree.min_left(3, at_most_two) == 1);
  tree.set(1, M::identity());
  assert(tree.fold().unwrap() == "ac");
  assert(pretty(tree) == "[\"a\", *, \"c\"]");
  assert(SegmentTree<M>{0}.fold().is_identity());
  assert(SegmentTree<M>{3}.fold().is_identity());

  FoldableQueue<M> queue;
  assert(queue.fold().is_identity());
  queue.push(Concat{"a"});
  queue.push(Concat{"b"});
  queue.push(M::identity());
  assert(queue.fold().unwrap() == "ab");
  queue.pop();
  assert(queue.fold().unwrap() == "b");
  queue.pop();
  assert(!queue.empty() && queue.fold().is_identity());
  queue.pop();
  assert(queue.empty() && queue.fold().is_identity());
}

int main() {
  const Concat value{"ab"};
  M a{value};
  const M b{Concat{"cd"}}, e = M::identity(), empty{Concat{std::string{}}};
  assert(M{}.is_identity() && e.is_identity());
  assert(!a.is_identity() && !empty.is_identity());
  assert(empty.unwrap().empty());
  assert((e * e).is_identity());
  assert(!(e * empty).is_identity() && !(empty * e).is_identity());
  assert((e * a).unwrap() == "ab" && (a * e).unwrap() == "ab");
  assert((a * b).unwrap() == "abcd" && (b * a).unwrap() == "cdab");
  assert(pretty(e) == "*" && pretty(a) == "\"ab\"");
  assert(pretty(empty) == "\"\"");
  const std::vector<M> values{e, empty, a, b};
  assert(pretty(values) == "[*, \"\", \"ab\", \"cd\"]");
  for (const M& x: values) {
    for (const M& y: values) {
      for (const M& z: values) {
        const M left = (x * y) * z, right = x * (y * z);
        assert(left.is_identity() == right.is_identity());
        if (!left.is_identity()) assert(left.unwrap() == right.unwrap());
      }
    }
  }

  a.unwrap() = "x";
  assert(std::as_const(a).unwrap() == "x" && value.unwrap() == "ab");
  M copied{a};
  copied.unwrap() = "y";
  assert(a.unwrap() == "x" && copied.unwrap() == "y");
  copied = e;
  assert(copied.is_identity());
  copied = a;
  assert(copied.unwrap() == "x");
  M moved{std::move(copied)};
  assert(moved.unwrap() == "x");
  moved = M::identity();
  assert(moved.is_identity());
  moved = M{Concat{"z"}};
  assert(moved.unwrap() == "z");

  UnitizationOfSemigroup<MutableOnly> mutable_only{MutableOnly{3}};
  mutable_only.unwrap() = 7;
  assert(mutable_only.unwrap() == 7);
  const UnitizationOfSemigroup<ConstOnly> const_only{ConstOnly{5}};
  assert(const_only.unwrap() == 5);
  const UnitizationOfSemigroup<mint> ma{mint{3}}, mb{mint{4}};
  assert((ma * mb).unwrap() == 12 && pretty(ma) == "3");
  assert(pretty(UnitizationOfSemigroup<int>{3}) == "3");
  assert(pretty(UnitizationOfSemigroup<int>{}) == "*");
  const UnitizationOfSemigroup<Opaque> opaque{Opaque{}};
  assert(!(opaque * opaque).is_identity());

  check_data_structures();
}
