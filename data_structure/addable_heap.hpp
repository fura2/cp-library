#pragma once

#include <cassert>
#include <concepts>
#include <cstddef>
#include <functional>
#include <queue>
#include <ranges>
#include <utility>
#include <vector>

namespace addable_heap_detail {

template <typename T, typename Compare>
class AddableHeap {
 public:
  AddableHeap(): pq{}, offset{} {}

  explicit AddableHeap(const std::vector<T>& a): pq{Compare{}, a}, offset{} {}

  explicit AddableHeap(std::vector<T>&& a)
      : pq{Compare{}, std::move(a)}, offset{} {}

  bool empty() const { return pq.empty(); }

  std::size_t size() const { return pq.size(); }

  void push(const T& x) { pq.push(x - offset); }

  template <typename R>
    requires std::ranges::input_range<R> &&
             std::constructible_from<T, std::ranges::range_reference_t<R>>
  void push_range(R&& rg) {
    std::vector<T> a;
    if constexpr (std::ranges::sized_range<R>) {
      a.reserve(std::ranges::size(rg));
    }
    for (auto&& x: std::forward<R>(rg)) {
      a.emplace_back(T{x} - offset);
    }
    pq.push_range(std::move(a));
  }

  T top() const {
    assert(!pq.empty());
    return pq.top() + offset;
  }

  void pop() {
    assert(!pq.empty());
    pq.pop();
  }

  void add_all(const T& x) { offset += x; }

 private:
  std::priority_queue<T, std::vector<T>, Compare> pq;
  T offset;
};

}  // namespace addable_heap_detail

template <typename T>
using AddableMaxHeap = addable_heap_detail::AddableHeap<T, std::less<T>>;

template <typename T>
using AddableMinHeap = addable_heap_detail::AddableHeap<T, std::greater<T>>;
