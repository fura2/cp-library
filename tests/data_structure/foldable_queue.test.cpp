#include "data_structure/foldable_queue.hpp"

#include <array>
#include <cassert>
#include <concepts>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "algebra/collection/add_group.hpp"
#include "algebra/monoid_impl.hpp"
#include "algebra/pair_monoid.hpp"

using Concat =
    MonoidImpl<std::string,
               [](const std::string& a, const std::string& b) { return a + b; },
               []() { return std::string{}; }>;

struct CountingSum {
  static inline int transfers = 0;
  int value;

  explicit CountingSum(int x): value{x} {}
  explicit CountingSum(std::unique_ptr<int>&& x): value{*x} { x.reset(); }
  CountingSum(const CountingSum& other): value{other.value} { ++transfers; }
  CountingSum(CountingSum&& other) noexcept: value{other.value} { ++transfers; }
  CountingSum& operator=(const CountingSum&) = default;
  CountingSum& operator=(CountingSum&&) = default;

  static CountingSum identity() { return CountingSum{0}; }
  friend CountingSum operator*(const CountingSum& a, const CountingSum& b) {
    return CountingSum{a.value + b.value};
  }
};

static_assert(Monoid<CountingSum>);
static_assert(std::constructible_from<CountingSum, const int&>);
static_assert(!std::convertible_to<int, CountingSum>);
static_assert(std::constructible_from<FoldableQueue<CountingSum>,
                                      std::vector<std::unique_ptr<int>>&&>);
static_assert(
    !std::constructible_from<FoldableQueue<CountingSum>,
                             const std::vector<std::unique_ptr<int>>&>);

void check_generic_inputs() {
  const std::vector<int> values{2, 3};
  FoldableQueue<CountingSum> copied(values);
  FoldableQueue<CountingSum> converted(std::vector<int>{2, 3});
  assert(copied.fold().value == 5 && converted.fold().value == 5);

  const std::vector<bool> bits{true, false, true};
  const FoldableQueue<CountingSum> copied_bits(bits);
  const FoldableQueue<CountingSum> moved_bits(
      std::vector<bool>{true, false, true});
  assert(copied_bits.fold().value == 2 && moved_bits.fold().value == 2);
  assert(copied_bits[1].value == 0 && moved_bits[1].value == 0);

  std::vector<std::unique_ptr<int>> source;
  source.push_back(std::make_unique<int>(2));
  source.push_back(std::make_unique<int>(5));
  FoldableQueue<CountingSum> moved(std::move(source));
  for (const auto& x: source) assert(!x);
  auto token = std::make_unique<int>(11);
  moved.push(std::move(token));
  assert(!token);
  std::vector<std::unique_ptr<int>> more;
  more.push_back(std::make_unique<int>(3));
  more.push_back(std::make_unique<int>(4));
  moved.push_range(more | std::views::as_rvalue);
  for (const auto& x: more) assert(!x);
  assert(moved.size() == 5 && moved.fold().value == 25);
  moved.pop();
  assert(moved.front().value == 5 && moved.fold().value == 23);
}

void check_repeated_ranges() {
  constexpr int n = 1024;
  FoldableQueue<CountingSum> q;
  CountingSum::transfers = 0;
  for (int i = 0; i < n; ++i) {
    q.push_range(std::array<int, 1>{i});
  }
  // Singleton ranges must not relocate the entire existing back stack each
  // time.
  assert(CountingSum::transfers < 8 * n);
  assert(q.size() == n && q.fold().value == n * (n - 1) / 2);
}

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

  using P = PairMonoid<Concat, AddGroup<int>>;
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

  check_generic_inputs();
  check_repeated_ranges();
}
