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
  explicit FoldableQueue(const std::vector<value_type>& v)
      : stk_back(v),
        cum_front{M::id()},
        cum_back{std::ranges::fold_left(stk_back, M::id(), M::op)} {}
  explicit FoldableQueue(std::vector<value_type>&& v)
      : stk_back(std::move(v)),
        cum_front{M::id()},
        cum_back{std::ranges::fold_left(stk_back, M::id(), M::op)} {}

  bool empty() const { return stk_front.empty() && stk_back.empty(); }
  size_type size() const { return stk_front.size() + stk_back.size(); }

  const value_type& front() const {
    return stk_front.empty() ? stk_back.front() : stk_front.back();
  }
  const value_type& back() const {
    return stk_back.empty() ? stk_front.front() : stk_back.back();
  }
  const value_type& operator[](size_type i) const {
    return i < stk_front.size() ? stk_front[stk_front.size() - i - 1]
                                : stk_back[i - stk_front.size()];
  }

  void push(const value_type& x) {
    const value_type& y = stk_back.emplace_back(x);
    cum_back = M::op(cum_back, y);
  }
  void push(value_type&& x) {
    const value_type& y = stk_back.emplace_back(std::move(x));
    cum_back = M::op(cum_back, y);
  }
  template <typename R>
    requires std::ranges::input_range<R> &&
             std::convertible_to<std::ranges::range_reference_t<R>, value_type>
  void push_range(R&& rg) {
    for (auto&& x: std::forward<R>(rg)) {
      const value_type& y = stk_back.emplace_back(std::forward<decltype(x)>(x));
      cum_back = M::op(cum_back, y);
    }
  }
  template <typename... Args>
  const value_type& emplace(Args&&... args) {
    const value_type& x = stk_back.emplace_back(std::forward<Args>(args)...);
    cum_back = M::op(cum_back, x);
    return x;
  }
  void pop() {
    if (stk_front.empty()) {
      stk_front.reserve(stk_back.size());
      cum_front.reserve(stk_back.size() + 1);
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

#ifdef DEBUG
  friend std::string pretty(const FoldableQueue& q) {
    std::string s = "[";
    for (size_type i = 0; i < q.size(); ++i) {
      s += (i == 0 ? "" : ", ") + pretty(q[i]);
    }
    s += "]";
    return s;
  }
#endif

 private:
  std::vector<value_type> stk_front, stk_back, cum_front;
  value_type cum_back;
};
