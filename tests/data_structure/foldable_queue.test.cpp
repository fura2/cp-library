#include "data_structure/foldable_queue.hpp"

#include <cassert>
#include <sstream>
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
  FoldableQueue<Concat> q;
  assert(q.empty() && q.size() == 0 && q.fold().unwrap().empty());
  q.push(std::string{"a"});
  assert(q.emplace("bc").unwrap() == "bc");
  q.push(std::string{"d"});
  assert(q.front().unwrap() == "a" && q.back().unwrap() == "d");
  assert(q.size() == 3 && q[1].unwrap() == "bc");
  assert(q.fold().unwrap() == "abcd");
  q.pop();  // Transfers the back stack to the front stack.
  assert(q.front().unwrap() == "bc" && q.fold().unwrap() == "bcd");
  q.push_range(std::vector<std::string>{"e", "f"});
  assert(q[1].unwrap() == "d" && q[2].unwrap() == "e");
  assert(q.back().unwrap() == "f" && q.fold().unwrap() == "bcdef");
  q.pop();
  q.pop();
  assert(q.fold().unwrap() == "ef");
  q.pop();
  q.pop();
  assert(q.empty() && q.fold().unwrap().empty());
  q.emplace("z");
  assert(q.front().unwrap() == "z" && q.back().unwrap() == "z");
  assert(pretty(q) == "[1 element(s)]");

  const std::vector<std::string> values = {"ab", "c"};
  FoldableQueue<Concat> copied{values};
  FoldableQueue<Concat> moved{std::vector<std::string>{"ab", "c"}};
  assert(copied.fold().unwrap() == "abc" && moved.fold().unwrap() == "abc");
  copied.pop();
  assert(copied.back().unwrap() == "c");
  std::istringstream in{"x y"};
  copied.push_range(std::ranges::istream_view<std::string>(in));
  assert(copied.fold().unwrap() == "cxy");

  using P = PairMonoid<Concat, IntAddGroup>;
  const std::vector<P> pair_values{{"ab", 2}, {"c", 3}};
  FoldableQueue<P> copied_pairs({pair_values});
  FoldableQueue<P> moved_pairs({{"ab", 2}, {"c", 3}});
  assert(copied_pairs.size() == 2 && moved_pairs.size() == 2);
  assert(copied_pairs.fold().first.unwrap() == "abc");
  assert(copied_pairs.fold().second.unwrap() == 5);
  assert(moved_pairs.fold().first.unwrap() == "abc");
  assert(moved_pairs.fold().second.unwrap() == 5);
  copied_pairs.pop();
  moved_pairs.pop();
  assert(copied_pairs.front().first.unwrap() == "c");
  assert(copied_pairs.fold().second.unwrap() == 3);
  assert(moved_pairs.front().first.unwrap() == "c");
  assert(moved_pairs.fold().second.unwrap() == 3);
  const FoldableQueue<P> empty_pairs({});
  assert(empty_pairs.empty());
  assert(empty_pairs.fold().first.unwrap().empty());
  assert(empty_pairs.fold().second.unwrap() == 0);

  FoldableQueue<P> pairs;
  pairs.push({"ab", 2});
  pairs.push_range({{"c", 3}, {"d", 4}});
  pairs.push_range({});
  assert(pairs.size() == 3);
  assert(pairs.fold().first.unwrap() == "abcd");
  assert(pairs.fold().second.unwrap() == 9);
  pairs.pop();
  const P tail{"e", 5};
  pairs.push(tail);
  assert(pairs.fold().first.unwrap() == "cde");
  assert(pairs.fold().second.unwrap() == 12);
}
