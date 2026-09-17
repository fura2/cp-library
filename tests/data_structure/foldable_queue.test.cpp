#include "data_structure/foldable_queue.hpp"

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#include "algebra/monoid_impl.hpp"

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
}
