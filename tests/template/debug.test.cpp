// Declare pretty overloads before templates that use them.
// clang-format off
#include "template/debug.hpp"
// clang-format on

#include <cassert>

#include "algebra/collection/add_group.hpp"
#include "data_structure/fenwick_tree.hpp"
#include "data_structure/foldable_queue.hpp"
#include "data_structure/segment_tree.hpp"
#include "tests/support/io.hpp"

int main() {
  assert(pretty(42) == "42" && pretty(-3) == "-3");
  assert(pretty(INF) == "∞" && pretty(-INF) == "-∞");
  assert(pretty(LINF) == "∞" && pretty(-LINF) == "-∞");
  assert(pretty(DINF) == "∞" && pretty(-DINF) == "-∞");
  assert(pretty(1U) == "1" && pretty(1L) == "1");
  assert(pretty(1UL) == "1" && pretty(1LL) == "1" && pretty(1ULL) == "1");
  assert(pretty(0.5F) == "0.500000");
  assert(pretty(0.5) == "0.500000" && pretty(0.5L) == "0.500000");
  assert(pretty(true) == "T" && pretty(false) == "F");
  assert(pretty('a') == "'a'" && pretty('\0') == "'\\0'");
  assert(pretty('\t') == "'\\t'" && pretty('\n') == "'\\n'");
  assert(pretty('\r') == "'\\r'");
  assert(pretty(static_cast<const char*>(nullptr)) == "null");
  assert(pretty("abc") == "\"abc\"" && pretty(std::string{"abc"}) == "\"abc\"");
  assert(pretty(std::optional<int>{}) == "*");
  assert(pretty(std::optional<int>{3}) == "3");
  assert(pretty(std::pair{1, 2}) == "(1, 2)");
  assert(pretty(std::tuple{1, true, std::string{"x"}}) == "(1, T, \"x\")");
  assert(pretty(std::tuple{}) == "()");
  const int a[] = {1, 2};
  assert(pretty(a) == "[1, 2]");
  assert(pretty(std::array<int, 0>{}) == "[]");
  assert(pretty(std::array{1, 2}) == "[1, 2]");
  assert(pretty(std::vector<int>{1, 2}) == "[1, 2]");
  assert(pretty(std::deque<int>{1, 2}) == "[1, 2]");
  assert(pretty(std::list<int>{1, 2}) == "[1, 2]");
  assert(pretty(std::set<int>{2, 1}) == "{1, 2}");
  assert(pretty(std::multiset<int>{2, 1, 1}) == "{1, 1, 2}");
  // Single-key inputs avoid depending on unordered-container iteration order.
  assert(pretty(std::unordered_set<int>{1}) == "{1}");
  assert(pretty(std::unordered_multiset<int>{1, 1}) == "{1, 1}");
  assert(pretty(std::map<int, int>{{2, 3}, {1, 4}}) == "{1: 4, 2: 3}");
  assert(pretty(std::multimap<int, int>{{1, 2}, {1, 3}}) == "{1: 2, 1: 3}");
  assert(pretty(std::unordered_map<int, int>{{1, 2}}) == "{1: 2}");
  assert(pretty(std::unordered_multimap<int, int>{{1, 2}, {1, 2}}) ==
         "{1: 2, 1: 2}");
  assert(pretty(std::vector<std::optional<std::pair<int, int>>>{
             std::pair{1, 2}, std::nullopt}) == "[(1, 2), *]");
  assert(pretty(std::pair{std::vector<int>{1, 2}, std::tuple{3, 4}}) ==
         "([1, 2], (3, 4))");

  const std::vector<IntAddGroup> values = {IntAddGroup{1}, IntAddGroup{2}};
  assert(pretty(SegmentTree<IntAddGroup>{values}) == "[1, 2]");
  assert(pretty(FenwickTree<IntAddGroup>{values}) == "[1, 2]");
  assert(pretty(FoldableQueue<IntAddGroup>{values}) == "[1, 2]");
  TestIO io;
  int x = 1, y = 2;
  show(x);
  show(x, y);
  show(x, y, x);
  show(x, y, x, y);
  show(x, y, x, y, x);
  show(x, y, x, y, x, y);
  show(x, y, x, y, x, y, x);
  assert(io.err.str() ==
         "\033[33mx = 1\n\033[0m"
         "\033[33mx = 1, y = 2\n\033[0m"
         "\033[33mx = 1, y = 2, x = 1\n\033[0m"
         "\033[33mx = 1, y = 2, x = 1, y = 2\n\033[0m"
         "\033[33mx = 1, y = 2, x = 1, y = 2, x = 1\n\033[0m"
         "\033[33mx = 1, y = 2, x = 1, y = 2, x = 1, y = 2\n\033[0m"
         "\033[33mx = 1, y = 2, x = 1, y = 2, x = 1, y = 2, x = 1\n\033[0m");
}
