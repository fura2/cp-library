#include "string/rolling_hash.hpp"

#include <array>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <forward_list>
#include <iterator>
#include <limits>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

template <typename R>
concept HashableRange = requires(R&& rg) {
  {
    RollingHash::hash(std::forward<R>(rg))
  } -> std::same_as<RollingHash::hash_type>;
};

static_assert(std::constructible_from<RollingHash, const std::string&>);
static_assert(std::constructible_from<RollingHash, std::vector<int>&>);
static_assert(std::constructible_from<RollingHash, std::vector<int>>);
static_assert(!std::convertible_to<std::string, RollingHash>);
static_assert(HashableRange<const std::string&>);
static_assert(HashableRange<std::vector<int>>);
static_assert(!std::constructible_from<RollingHash, std::forward_list<int>&>);
static_assert(!HashableRange<std::forward_list<int>&>);
static_assert(!std::constructible_from<RollingHash, int>);
static_assert(!HashableRange<int>);

// A sized input range whose elements are consumed by iteration.
struct ConsumingRange {
  std::span<const int> remaining;

  struct Iterator {
    using value_type = int;
    using difference_type = std::ptrdiff_t;

    ConsumingRange* source;

    int operator*() const { return source->remaining.front(); }
    Iterator& operator++() {
      source->remaining = source->remaining.subspan(1);
      return *this;
    }
    void operator++(int) { ++*this; }
    bool operator==(std::default_sentinel_t) const {
      return source->remaining.empty();
    }
  };

  Iterator begin() { return {this}; }
  std::default_sentinel_t end() const { return {}; }
  auto size() const { return remaining.size(); }
};

static_assert(std::ranges::input_range<ConsumingRange>);
static_assert(std::ranges::sized_range<ConsumingRange>);
static_assert(!std::ranges::forward_range<ConsumingRange>);
static_assert(std::constructible_from<RollingHash, ConsumingRange&>);
static_assert(HashableRange<ConsumingRange&>);

template <typename R>
void check_subranges(const R& values) {
  const RollingHash hash{values};
  const int n = std::ranges::size(values);
  assert(hash.size() == n);
  assert(hash.hash().size() == n);
  assert(hash.hash() == RollingHash::hash(values));
  assert(hash.hash() == hash.hash(0, n));

  for (int l = 0; l <= n; ++l) {
    for (int r = l; r <= n; ++r) {
      const auto slice = std::span{values}.subspan(l, r - l);
      const auto value = hash.hash(l, r);
      assert(value.size() == r - l);
      assert(value == RollingHash{slice}.hash());
      assert(value == RollingHash::hash(slice));
      for (int m = l; m <= r; ++m) {
        assert(hash.hash(l, m) * hash.hash(m, r) == value);
      }
    }
  }
}

int main() {
  check_subranges(std::string{});
  check_subranges(std::string{"x"});
  check_subranges(std::string{"abacaba"});
  check_subranges(std::string{"a\0b\0a", 5});
  check_subranges(std::string(16, 'z'));
  check_subranges(std::vector<int>{});
  check_subranges(std::vector<int>{0, 0, 0});
  check_subranges(std::vector<int>{-1,
                                   0,
                                   1,
                                   std::numeric_limits<int>::min(),
                                   std::numeric_limits<int>::max(),
                                   -42,
                                   42,
                                   -1});
  check_subranges(std::vector<unsigned int>{
      0, 1, std::numeric_limits<unsigned int>::max(), 7, 0});

  const RollingHash empty;
  const auto id = empty.hash();
  assert(empty.size() == 0 && id.size() == 0);
  assert(id == empty.hash(0, 0));
  assert(id == RollingHash::hash(std::string_view{}));
  assert(id * id == id);

  const auto a = RollingHash::hash(std::string_view{"a"});
  const auto b = RollingHash::hash(std::string_view{"b"});
  const auto c = RollingHash::hash(std::string_view{"cde"});
  assert(id * a == a && a * id == a);
  assert(a != b);
  assert(a * b != b * a);  // Distinct for every allowed base.
  assert((a * b) * c == a * (b * c));
  assert((a * b) * c == RollingHash::hash(std::string_view{"abcde"}));
  assert(RollingHash::hash(std::array{0}) !=
         RollingHash::hash(std::array{0, 0}));

  const int values[] = {1, -2, 3};
  check_subranges(values);
  const auto expected = RollingHash::hash(values);
  auto transformed =
      values |
      std::views::transform([offset = 0](int x) mutable { return x + offset; });
  static_assert(!std::ranges::range<const decltype(transformed)>);
  assert(RollingHash{transformed}.hash() == expected);
  assert(RollingHash::hash(transformed) == expected);

  auto owned = std::views::all(std::vector<int>{1, -2, 3});
  static_assert(!std::copy_constructible<decltype(owned)>);
  assert(RollingHash{owned}.hash() == expected);
  assert(RollingHash::hash(owned) == expected);
  assert(RollingHash{std::move(owned)}.hash() == expected);
  assert(RollingHash::hash(std::views::all(std::vector<int>{1, -2, 3})) ==
         expected);

  const auto filtered =
      values | std::views::filter([](int x) { return x > 0; });
  static_assert(!std::constructible_from<RollingHash, decltype(filtered)&>);
  static_assert(!HashableRange<decltype(filtered)&>);

  for (const auto input:
       {std::span<const int>{}, std::span<const int>{values}}) {
    const auto direct = RollingHash::hash(input);
    ConsumingRange for_static{input}, for_constructor{input};
    const auto value = RollingHash::hash(for_static);
    const RollingHash hash{for_constructor};
    assert(for_static.size() == 0 && for_constructor.size() == 0);
    assert(value.size() == static_cast<int>(input.size()));
    assert(hash.size() == static_cast<int>(input.size()));
    assert(value == direct && hash.hash() == direct);
    assert(hash.hash(0, hash.size()) == direct);
  }

  const RollingHash small{std::string_view{"abc"}};
  const auto saved = small.hash(1, 3);
  std::string repeated = "ab";
  auto repeated_hash = RollingHash::hash(repeated);
  for (int i = 0; i < 10; ++i) {
    repeated += repeated;
    repeated_hash = repeated_hash * repeated_hash;
    assert(repeated_hash.size() == static_cast<int>(repeated.size()));
    assert(repeated_hash == RollingHash::hash(repeated));
  }
  const RollingHash large{repeated};
  assert(large.hash(1, large.size()) ==
         RollingHash::hash(std::string_view{repeated}.substr(1)));
  assert(small.hash(1, 3) ==
         saved);  // Growing shared powers preserves old hashes.
}
