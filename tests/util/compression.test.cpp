#include "util/compression.hpp"

#include <array>
#include <cassert>
#include <compare>
#include <concepts>
#include <memory>
#include <ranges>
#include <sstream>
#include <utility>
#include <vector>

struct MoveOnlyValue {
  static inline int transfers = 0;
  std::unique_ptr<int> value;

  explicit MoveOnlyValue(int x): value{std::make_unique<int>(x)} {}
  MoveOnlyValue(const MoveOnlyValue&) = delete;
  MoveOnlyValue& operator=(const MoveOnlyValue&) = delete;
  MoveOnlyValue(MoveOnlyValue&& other) noexcept: value{std::move(other.value)} {
    ++transfers;
  }
  MoveOnlyValue& operator=(MoveOnlyValue&&) noexcept = default;

  auto operator<=>(const MoveOnlyValue& other) const {
    return *value <=> *other.value;
  }
  bool operator==(const MoveOnlyValue& other) const {
    return *value == *other.value;
  }
};

static_assert(!std::copy_constructible<MoveOnlyValue>);
static_assert(std::totally_ordered<MoveOnlyValue>);
static_assert(std::constructible_from<MoveOnlyValue, const int&>);
static_assert(!std::convertible_to<int, MoveOnlyValue>);
static_assert(std::constructible_from<Compression<MoveOnlyValue>,
                                      std::vector<MoveOnlyValue>&&>);
static_assert(!std::constructible_from<Compression<MoveOnlyValue>,
                                       const std::vector<MoveOnlyValue>&>);

void check_values_and_indices() {
  Compression<int> empty;
  empty.build();
  assert(empty.size() == 0 && empty.index(0) == 0);
  Compression<int> empty_list({});
  empty_list.add_range({});
  empty_list.build();
  assert(empty_list.size() == 0);

  Compression<int> single({5});
  single.add(5);
  single.build();
  assert(single.size() == 1 && single.value(0) == 5);
  assert(single.index(4) == 0 && single.index(5) == 0 && single.index(6) == 1);

  const std::vector<int> values{-3, 4, -3, 1};
  Compression<int> c(values);
  c.add(0);
  c.add_range({4, 7});
  c.add_range({});
  c.build();
  const std::array expected{-3, 0, 1, 4, 7};
  assert(c.size() == static_cast<int>(expected.size()));
  for (int i = 0; i < c.size(); ++i) {
    assert(c.value(i) == expected[i] && c.index(expected[i]) == i);
  }
  assert(c.index(-4) == 0 && c.index(2) == 3 && c.index(8) == 5);

  Compression<std::pair<int, int>> pairs({{2, 3}, {1, 2}, {2, 3}});
  pairs.add({3, 4});
  pairs.add_range({{0, 1}, {1, 2}});
  pairs.build();
  assert(pairs.size() == 4 && pairs.index({2, 3}) == 2);
  assert((pairs.value(0) == std::pair{0, 1}));
  assert((pairs.value(3) == std::pair{3, 4}));
}

void check_conversions_and_moves() {
  const std::vector<int> values{3, 1, 3};
  Compression<MoveOnlyValue> copied(values);
  copied.add_range(values);
  copied.build();
  assert(copied.size() == 2);
  assert(*copied.value(0).value == 1 && *copied.value(1).value == 3);
  Compression<MoveOnlyValue> converted(std::vector<int>{3, 1, 3});
  converted.build();
  assert(converted.size() == 2 && *converted.value(1).value == 3);

  std::vector<MoveOnlyValue> source;
  source.emplace_back(3);
  source.emplace_back(1);
  Compression<MoveOnlyValue> moved(std::move(source));
  for (const auto& x: source) assert(!x.value);
  MoveOnlyValue token{2};
  moved.add(std::move(token));
  assert(!token.value);
  moved.add(4);

  std::vector<MoveOnlyValue> more;
  more.emplace_back(3);
  more.emplace_back(5);
  moved.add_range(more | std::views::as_rvalue);
  for (const auto& x: more) assert(!x.value);
  std::istringstream input{"-1 4"};
  auto stream = std::ranges::istream_view<int>(input);
  static_assert(std::ranges::input_range<decltype(stream)>);
  static_assert(!std::ranges::sized_range<decltype(stream)>);
  moved.add_range(stream);
  moved.build();
  const std::array expected{-1, 1, 2, 3, 4, 5};
  assert(moved.size() == static_cast<int>(expected.size()));
  for (int i = 0; i < moved.size(); ++i) {
    assert(*moved.value(i).value == expected[i]);
  }
  assert(moved.index(MoveOnlyValue{3}) == 3);

  const std::vector<bool> bits{true, false, true};
  Compression<int> copied_bits(bits);
  Compression<int> moved_bits(std::vector<bool>{true, false, true});
  copied_bits.build();
  moved_bits.build();
  assert(copied_bits.size() == 2 && moved_bits.size() == 2);
  assert(copied_bits.value(0) == 0 && copied_bits.value(1) == 1);
  assert(moved_bits.value(0) == 0 && moved_bits.value(1) == 1);
}

void check_repeated_ranges() {
  constexpr int n = 1024;
  Compression<MoveOnlyValue> c;
  MoveOnlyValue::transfers = 0;
  for (int i = 0; i < n; ++i) {
    c.add_range(std::array<int, 1>{i});
  }
  // Singleton ranges must not relocate all previously stored values each time.
  assert(MoveOnlyValue::transfers < 8 * n);
  c.build();
  assert(c.size() == n);
  assert(*c.value(0).value == 0 && *c.value(n - 1).value == n - 1);
}

int main() {
  check_values_and_indices();
  check_conversions_and_moves();
  check_repeated_ranges();
}
