#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <ranges>
#include <utility>
#include <vector>

template <typename T>
class Compression {
 public:
  explicit Compression(): a{}, built{false} {}

  template <typename U = T>
    requires std::constructible_from<T, const U&>
  explicit Compression(const std::vector<U>& a): built{false} {
    this->a.reserve(a.size());
    for (const auto& x: a) {
      this->a.emplace_back(x);
    }
  }

  template <typename U = T>
    requires std::constructible_from<T, U&&>
  explicit Compression(std::vector<U>&& a): built{false} {
    this->a.reserve(a.size());
    for (auto&& x: a) {
      this->a.emplace_back(std::move(x));
    }
  }

  template <typename U = T>
    requires std::constructible_from<T, U&&>
  void add(U&& v) {
    assert(!built);
    a.emplace_back(std::forward<U>(v));
  }

  template <typename R = std::initializer_list<T>>
    requires std::ranges::input_range<R> &&
             std::constructible_from<T, std::ranges::range_reference_t<R>>
  void add_range(R&& rg) {
    assert(!built);
    for (auto&& x: std::forward<R>(rg)) {
      a.emplace_back(std::forward<decltype(x)>(x));
    }
  }

  int size() const {
    if (!built) build();
    return a.size();
  }

  const T& value(int i) const {
    if (!built) build();
    assert(0 <= i && i < size());
    return a[i];
  }

  int index(const T& v) const {
    if (!built) build();
    return std::ranges::lower_bound(a, v) - a.begin();
  }

 private:
  mutable std::vector<T> a;
  mutable bool built;

  void build() const {
    std::ranges::sort(a);
    a.erase(std::ranges::unique(a).begin(), a.end());
    built = true;
  }
};
