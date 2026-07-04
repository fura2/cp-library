#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "algebra/monoid.hpp"

template <Monoid M>
class FoldableQueue {
 public:
  FoldableQueue(): cum_front{M::identity()}, cum_back{M::identity()} {}
  template <typename T>
    requires std::constructible_from<M, T>
  FoldableQueue(const std::vector<T>& v)
      : stk_back(v.begin(), v.end()),
        cum_front{M::identity()},
        cum_back{std::ranges::fold_left(
            stk_back, M::identity(), std::multiplies{})} {}

  bool empty() const { return stk_front.empty() && stk_back.empty(); }
  std::size_t size() const { return stk_front.size() + stk_back.size(); }

  const M& front() const {
    return stk_front.empty() ? stk_back.front() : stk_front.back();
  }
  const M& back() const {
    return stk_back.empty() ? stk_front.front() : stk_back.back();
  }
  const M& operator[](int i) const {
    return i < stk_front.size() ? stk_front[stk_front.size() - i - 1]
                                : stk_back[i - stk_front.size()];
  }

  void push(const M& x) {
    stk_back.emplace_back(x);
    cum_back *= x;
  }
  template <typename R>
    requires std::ranges::input_range<R> &&
             std::convertible_to<std::ranges::range_reference_t<R>, M>
  void push_range(R&& rg) {
    for (auto&& x: std::forward<R>(rg)) {
      const M& y = stk_back.emplace_back(std::forward<decltype(x)>(x));
      cum_back *= y;
    }
  }
  template <typename... Args>
  const M& emplace(Args&&... args) {
    const M& x = stk_back.emplace_back(std::forward<Args>(args)...);
    cum_back *= x;
    return x;
  }
  void pop() {
    if (stk_front.empty()) {
      stk_front.reserve(stk_back.size());
      cum_front.reserve(stk_back.size() + 1);
      while (!stk_back.empty()) {
        stk_front.emplace_back(std::move(stk_back.back()));
        cum_front.emplace_back(stk_front.back() * cum_front.back());
        stk_back.pop_back();
      }
      cum_back = M::identity();
    }
    stk_front.pop_back();
    cum_front.pop_back();
  }

  M fold() const { return cum_front.back() * cum_back; }

  friend std::string pretty(const FoldableQueue& q)
    requires requires(const M& x) {
      { pretty(x) } -> std::convertible_to<std::string>;
    }
  {
    std::string s = "[";
    for (int i = 0; i < q.stk_front.size(); ++i) {
      s += (i == 0 ? "" : ", ") +
           pretty(q.stk_front[q.stk_front.size() - i - 1]);
    }
    s += " | ";
    for (int i = 0; i < q.stk_back.size(); ++i) {
      s += (i == 0 ? "" : ", ") + pretty(q.stk_back[i]);
    }
    s += "]";
    return s;
  }

 private:
  std::vector<M> stk_front, stk_back, cum_front;
  M cum_back;
};
