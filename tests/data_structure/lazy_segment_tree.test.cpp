// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include "data_structure/lazy_segment_tree.hpp"

#include <cassert>
#include <concepts>
#include <string>
#include <utility>
#include <vector>

#include "algebra/collection/last_monoid.hpp"
#include "algebra/monoid_action_impl.hpp"
#include "algebra/monoid_impl.hpp"

using P = std::pair<long long, long long>;

// Values store (sum, length); actions store the coefficients of x -> a*x+b.
using SumLength = MonoidImpl<P,
                             [](const P& x, const P& y) {
                               return P{x.first + y.first, x.second + y.second};
                             },
                             [] { return P{0, 0}; }>;
using Affine =
    MonoidImpl<P,
               [](const P& f, const P& g) {
                 return P{g.first * f.first, g.first * f.second + g.second};
               },
               [] { return P{1, 0}; }>;
using AffineSum =
    MonoidActionImpl<SumLength,
                     Affine,
                     [](const SumLength& m, const Affine& f) {
                       const auto [sum, len] = m.unwrap();
                       const auto [a, b] = f.unwrap();
                       return SumLength{P{a * sum + b * len, len}};
                     }>;
using Tree = LazySegmentTree<AffineSum>;

using AffinePoint = MonoidActionImpl<long long,
                                     Affine,
                                     [](const long long& x, const Affine& f) {
                                       const auto [a, b] = f.unwrap();
                                       return a * x + b;
                                     }>;

template <typename A>
concept ValidLazySegmentTree = requires { typename LazySegmentTree<A>; };

static_assert(MonoidActionOnMonoid<AffineSum>);
static_assert(ValidLazySegmentTree<AffineSum>);
static_assert(MonoidAction<AffinePoint> && !MonoidActionOnMonoid<AffinePoint>);
static_assert(!ValidLazySegmentTree<AffinePoint>);
static_assert(!ValidLazySegmentTree<int>);

void check_sums(const Tree& tree, const std::vector<long long>& values) {
  const int n = values.size();
  assert(tree.size() == n);
  long long total = 0;
  for (const auto x: values) total += x;
  // Check the root before queries push any pending actions down.
  assert((tree.fold().unwrap() == P{total, n}));
  for (int l = 0; l <= n; ++l) {
    long long sum = 0;
    for (int r = l; r <= n; ++r) {
      assert((tree.fold(l, r).unwrap() == P{sum, r - l}));
      if (r < n) sum += values[r];
    }
  }
  assert((tree.fold().unwrap() == P{total, n}));
}

void check_affine_sum() {
  Tree empty{0};
  empty.apply(0, 0, P{2, 3});
  check_sums(empty, {});
  check_sums(Tree{std::vector<P>{}}, {});

  const Tree identities{3};
  assert(identities.size() == 3);
  assert((identities.fold().unwrap() == P{0, 0}));
  assert((identities.fold(1, 3).unwrap() == P{0, 0}));

  Tree single{std::vector<SumLength>{SumLength{P{4, 1}}}};
  single.apply(0, 1, P{0, -2});
  single.apply(0, 1, Affine{P{3, 1}});
  check_sums(single, {-5});

  Tree tree{std::vector<P>{{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}};
  static_assert(std::same_as<decltype(tree.fold()), const SumLength&>);
  static_assert(std::same_as<decltype(tree.fold(0, 5)), SumLength>);
  check_sums(tree, {1, 2, 3, 4, 5});

  // Do not query subranges between updates: pending actions must compose.
  tree.apply(0, 5, P{2, 1});
  assert((tree.fold().unwrap() == P{35, 5}));
  tree.apply(0, 4, Affine{P{3, 2}});
  assert((tree.fold().unwrap() == P{91, 5}));

  // Copy and assign while descendants still have pending actions.
  const Tree snapshot = tree;
  Tree copied{0};
  copied = tree;
  Tree moved{1};
  moved = std::move(copied);

  const P assign_seven{0, 7};
  tree.apply(1, 5, assign_seven);
  tree.apply(2, 4, P{1, -3});
  tree.apply(0, 5, Affine::identity());
  tree.apply(0, 0, Affine{P{0, 100}});
  tree.apply(3, 3, P{0, 100});
  tree.apply(5, 5, Affine{P{0, 100}});
  check_sums(tree, {11, 7, 4, 4, 7});
  check_sums(snapshot, {11, 17, 23, 29, 11});
  check_sums(moved, {11, 17, 23, 29, 11});

  moved = Tree{0};
  check_sums(moved, {});
}

void check_point_access() {
  Tree single{1};
  static_assert(
      std::same_as<decltype(std::as_const(single).get(0)), const SumLength&>);
  assert((single.get(0).unwrap() == P{0, 0}));
  single.set(0, P{4, 1});
  single.apply(0, 1, P{2, 1});
  assert((single.get(0).unwrap() == P{9, 1}));
  single.set(0, SumLength{P{-3, 1}});
  check_sums(single, {-3});

  Tree tree{std::vector<P>{{1, 1}, {2, 1}, {3, 1}, {4, 1}}};
  tree.apply(0, 4, P{2, 1});
  tree.apply(0, 4, P{3, 2});
  assert((tree.fold().unwrap() == P{80, 4}));
  // Read both ends before any range query propagates their pending actions.
  const Tree& view = tree;
  assert((view.get(3).unwrap() == P{29, 1}));
  assert((view.get(0).unwrap() == P{11, 1}));
  assert((view.get(3).unwrap() == P{29, 1}));
  assert((tree.fold().unwrap() == P{80, 4}));

  // A point assignment must override earlier pending actions, then receive
  // subsequent updates normally. Braces also exercise the default T = X.
  tree.apply(0, 4, P{0, 7});
  tree.set(2, {P{100, 1}});
  assert((tree.fold().unwrap() == P{121, 4}));
  tree.apply(1, 4, P{2, 1});
  assert((tree.fold().unwrap() == P{238, 4}));
  tree.set(0, P{-4, 1});
  tree.set(3, P{6, 1});
  check_sums(tree, {-4, 15, 201, 6});
}

void check_empty_boundaries() {
  // Include powers of two, where the end boundary is past the last leaf.
  for (int n: {0, 1, 2, 3, 4, 5, 8}) {
    Tree tree{std::vector<P>(n, P{2, 1})};
    tree.apply(0, n, P{3, 1});
    for (int i = 0; i <= n; ++i) {
      tree.apply(i, i, P{0, 99});
      assert((std::as_const(tree).fold(i, i).unwrap() == P{0, 0}));
      assert((tree.fold().unwrap() == P{7LL * n, n}));
    }
    check_sums(tree, std::vector<long long>(n, 7));
  }
}

void check_concatenation() {
  using Word = MonoidImpl<std::string,
                          [](const std::string& a, const std::string& b) {
                            return a + b;
                          },
                          [] { return std::string{}; }>;
  using Paint =
      MonoidActionImpl<Word,
                       LastMonoid<char>,
                       [](const Word& m, const LastMonoid<char>& f) {
                         return f.unwrap().has_value()
                                    ? Word{std::string(m.unwrap().size(),
                                                       *f.unwrap())}
                                    : m;
                       }>;
  LazySegmentTree<Paint> tree{
      std::vector<std::string>{"ab", "c", "de", "", "fg"}};
  assert(tree.fold().unwrap() == "abcdefg");
  tree.apply(0, 4, 'x');
  tree.apply(0, 4, LastMonoid<char>{'y'});
  tree.apply(1, 3, 'z');
  assert(tree.fold().unwrap() == "yyzzzfg");
  assert(std::as_const(tree).fold(0, 3).unwrap() == "yyzzz");
  assert(std::as_const(tree).fold(1, 5).unwrap() == "zzzfg");
  assert(std::as_const(tree).fold(2, 4).unwrap() == "zz");
  assert(std::as_const(tree).fold(3, 4).unwrap().empty());
  assert(std::as_const(tree).fold(2, 2).unwrap().empty());

  // Replacing a word changes its length as well as the noncommutative fold.
  tree.apply(0, 5, 'q');
  tree.set(1, std::string{"XYZ"});
  assert(tree.fold().unwrap() == "qqXYZqqqq");
  tree.apply(1, 4, 'r');
  assert(tree.fold().unwrap() == "qqrrrrrqq");
  assert(std::as_const(tree).get(1).unwrap() == "rrr");
  assert(std::as_const(tree).fold(1, 5).unwrap() == "rrrrrqq");
}

struct ExplicitScale {
  int factor;

  explicit ExplicitScale(int factor): factor{factor} {}

  friend ExplicitScale operator*(const ExplicitScale& f,
                                 const ExplicitScale& g) {
    return ExplicitScale{f.factor * g.factor};
  }
  static ExplicitScale identity() { return ExplicitScale{1}; }
};

void check_explicit_action() {
  static_assert(std::constructible_from<ExplicitScale, const int&>);
  static_assert(!std::convertible_to<const int&, ExplicitScale>);

  using ScaleSum =
      MonoidActionImpl<SumLength,
                       ExplicitScale,
                       [](const SumLength& m, const ExplicitScale& f) {
                         const auto [sum, len] = m.unwrap();
                         return SumLength{P{sum * f.factor, len}};
                       }>;
  LazySegmentTree<ScaleSum> tree{
      std::vector<P>{{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}};
  tree.apply(0, 5, 2);
  tree.apply(1, 4, 3);
  assert((tree.fold().unwrap() == P{66, 5}));
  assert((tree.fold(1, 3).unwrap() == P{30, 2}));
  assert((tree.fold(4, 5).unwrap() == P{10, 1}));
}

void check_pretty() {
  assert(pretty(Tree{0}) == "[]");
  Tree tree{std::vector<P>{{1, 1}, {2, 1}, {3, 1}, {4, 1}}};
  tree.apply(0, 4, P{0, 5});
  assert(pretty(std::as_const(tree)) == "[(5, 1), (5, 1), (5, 1), (5, 1)]");
  assert((tree.fold().unwrap() == P{20, 4}));
  tree.set(2, P{9, 1});
  assert(pretty(std::as_const(tree)) == "[(5, 1), (5, 1), (9, 1), (5, 1)]");
  check_sums(tree, {5, 5, 9, 5});
}

int main() {
  // max_right/min_left are not implemented yet.
  check_affine_sum();
  check_point_access();
  check_empty_boundaries();
  check_concatenation();
  check_explicit_action();
  check_pretty();
}
