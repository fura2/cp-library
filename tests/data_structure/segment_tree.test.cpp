#include "data_structure/segment_tree.hpp"

#include <cassert>
#include <concepts>
#include <string>
#include <vector>

#include "algebra/collection/add_group.hpp"
#include "algebra/monoid_impl.hpp"
#include "algebra/pair_monoid.hpp"

using Concat =
    MonoidImpl<std::string,
               [](const std::string& a, const std::string& b) { return a + b; },
               []() { return std::string{}; }>;

int main() {
  auto at_most = [](int limit) {
    return [limit](const IntAddGroup& x) { return x.unwrap() <= limit; };
  };
  const SegmentTree<IntAddGroup> empty{0};
  static_assert(std::same_as<decltype(empty.fold()), const IntAddGroup&>);
  assert(empty.size() == 0 && empty.fold().unwrap() == 0);
  assert(empty.fold(0, 0).unwrap() == 0);
  assert(empty.max_right(0, at_most(0)) == 0);
  assert(empty.min_left(0, at_most(0)) == 0);
  SegmentTree<IntAddGroup> single{1};
  single.set(0, 7);
  assert(single.get(0).unwrap() == 7 && single.fold().unwrap() == 7);

  SegmentTree<IntAddGroup> tree{std::vector<int>{2, 1, 3, 0, 4}};
  static_assert(std::same_as<decltype(tree.fold()), const IntAddGroup&>);
  static_assert(std::same_as<decltype(tree.fold(0, 5)), IntAddGroup>);
  assert(tree.size() == 5 && tree.fold().unwrap() == 10);
  assert(tree.fold(1, 4).unwrap() == 4 && tree.fold(2, 2).unwrap() == 0);
  assert(tree.max_right(0, at_most(3)) == 2);
  assert(tree.max_right(1, at_most(4)) == 4);
  assert(tree.max_right(0, at_most(0)) == 0);
  assert(tree.max_right(0, at_most(100)) == 5);
  assert(tree.max_right(5, at_most(0)) == 5);
  assert(tree.min_left(5, at_most(4)) == 3);
  assert(tree.min_left(4, at_most(4)) == 1);
  assert(tree.min_left(5, at_most(0)) == 5);
  assert(tree.min_left(5, at_most(100)) == 0);
  assert(tree.min_left(0, at_most(0)) == 0);
  tree.set(2, 6);
  assert(tree.get(2).unwrap() == 6 && tree.fold().unwrap() == 13);
  assert(tree.fold(1, 4).unwrap() == 7);
  assert(pretty(tree) == "[5 element(s)]");

  SegmentTree<Concat> strings{
      std::vector<std::string>{"a", "bc", "d", "e", "f"}};
  assert(strings.fold().unwrap() == "abcdef");
  assert(strings.fold(1, 4).unwrap() == "bcde");
  assert(strings.max_right(1, [](const Concat& x) {
    return std::string{"bcde"}.starts_with(x.unwrap());
  }) == 4);
  assert(strings.min_left(4, [](const Concat& x) {
    return std::string{"bcde"}.ends_with(x.unwrap());
  }) == 1);
  const auto& all = strings.fold();
  const auto snapshot = strings.fold();
  strings.set(2, std::string{"XY"});
  assert(strings.fold().unwrap() == "abcXYef");
  assert(all.unwrap() == "abcXYef");
  assert(snapshot.unwrap() == "abcdef");

  using P = PairMonoid<Concat, IntAddGroup>;
  SegmentTree<P> pairs({{"a", 1}, {"bc", 2}, {"d", 3}});
  assert(pairs.size() == 3);
  assert(pairs.fold().first.unwrap() == "abcd");
  assert(pairs.fold().second.unwrap() == 6);
  pairs.set(1, {"XY", 5});
  assert(pairs.fold().first.unwrap() == "aXYd");
  assert(pairs.fold().second.unwrap() == 9);
  assert(pairs.fold(1, 3).first.unwrap() == "XYd");
  assert(pairs.fold(1, 3).second.unwrap() == 8);
}
