#pragma once

#include <algorithm>
#include <concepts>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "algebra/monoid.hpp"

template <Monoid M>
class FoldableQueue {
 public:
  using value_type = M::value_type;
  using size_type = std::vector<value_type>::size_type;

  FoldableQueue(): cum_front{M::id()}, cum_back{M::id()} {}
  FoldableQueue(const std::vector<value_type>& v)
      : stk_back{v},
        cum_front{M::id()},
        cum_back{std::ranges::fold_left(v, M::id(), M::op)} {}
  FoldableQueue(std::vector<value_type>&& v)
      : stk_back{v},
        cum_front{M::id()},
        cum_back{std::ranges::fold_left(v, M::id(), M::op)} {}

  bool empty() const { return stk_front.empty() && stk_back.empty(); }
  size_type size() const { return stk_front.size() + stk_back.size(); }

  value_type& front() {
    return stk_front.empty() ? stk_back.front() : stk_front.back();
  }
  const value_type& front() const {
    return stk_front.empty() ? stk_back.front() : stk_front.back();
  }
  value_type& back() {
    return stk_back.empty() ? stk_front.front() : stk_back.back();
  }
  const value_type& back() const {
    return stk_back.empty() ? stk_front.front() : stk_back.back();
  }
  value_type& operator[](size_type i) {
    return i < stk_front.size() ? stk_front[stk_front.size() - i - 1]
                                : stk_back[i - stk_front.size()];
  }
  const value_type& operator[](size_type i) const {
    return i < stk_front.size() ? stk_front[stk_front.size() - i - 1]
                                : stk_back[i - stk_front.size()];
  }

  void push(const value_type& x) {
    stk_back.emplace_back(x);
    cum_back = M::op(cum_back, x);
  }
  void push(value_type&& x) {
    stk_back.emplace_back(x);
    cum_back = M::op(cum_back, x);
  }
  template <typename R>
    requires std::ranges::input_range<R> &&
             std::convertible_to<std::ranges::range_reference_t<R>, value_type>
  void push_range(R&& rg) {
    stk_back.append_range(rg);
    cum_back = std::ranges::fold_left(rg, cum_back, M::op);
  }
  template <typename... Args>
  decltype(auto) emplace(Args&&... args) {
    auto x = stk_back.emplace_back(args...);
    cum_back = M::op(cum_back, x);
  }
  void pop() {
    if (stk_front.empty()) {
      while (!stk_back.empty()) {
        stk_front.emplace_back(std::move(stk_back.back()));
        cum_front.emplace_back(M::op(stk_front.back(), cum_front.back()));
        stk_back.pop_back();
      }
      cum_back = M::id();
    }
    stk_front.pop_back();
    cum_front.pop_back();
  }

  value_type fold() const { return M::op(cum_front.back(), cum_back); }

  void swap(FoldableQueue& q) noexcept {
    stk_front.swap(q.stk_front);
    stk_back.swap(q.stk_back);
    cum_front.swap(q.cum_front);
    std::swap(cum_back, q.cum_back);
  }
  friend void swap(FoldableQueue& q1, FoldableQueue& q2) noexcept {
    q1.swap(q2);
  }

  friend std::string pretty(const FoldableQueue& q) {
    std::string s = "[";
    for (FoldableQueue::size_type i = 0; i < q.size(); ++i) {
      s += (i == 0 ? "" : ", ") + pretty(q[i]);
    }
    s += "]";
    return s;
  }

 private:
  std::vector<value_type> stk_front, stk_back, cum_front;
  value_type cum_back;
};
