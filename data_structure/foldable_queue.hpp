#pragma once

#include <concepts>
#include <cstddef>
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
    requires std::constructible_from<M, const T&>
  explicit FoldableQueue(const std::vector<T>& a)
      : cum_front{M::identity()}, cum_back{M::identity()} {
    stk_back.reserve(a.size());
    for (const T& x: a) {
      const M& y = stk_back.emplace_back(x);
      cum_back = cum_back * y;
    }
  }

  template <typename T>
    requires std::constructible_from<M, T&&>
  explicit FoldableQueue(std::vector<T>&& a)
      : cum_front{M::identity()}, cum_back{M::identity()} {
    stk_back.reserve(a.size());
    for (T& x: a) {
      const M& y = stk_back.emplace_back(std::move(x));
      cum_back = cum_back * y;
    }
  }

  bool empty() const { return stk_front.empty() && stk_back.empty(); }

  std::size_t size() const { return stk_front.size() + stk_back.size(); }

  const M& front() const {
    return stk_front.empty() ? stk_back.front() : stk_front.back();
  }

  const M& back() const {
    return stk_back.empty() ? stk_front.front() : stk_back.back();
  }

  const M& operator[](std::size_t i) const {
    return i < stk_front.size() ? stk_front[stk_front.size() - i - 1]
                                : stk_back[i - stk_front.size()];
  }

  template <typename T>
    requires std::constructible_from<M, T&&>
  void push(T&& x) {
    const M& y = stk_back.emplace_back(std::forward<T>(x));
    cum_back = cum_back * y;
  }

  template <typename R>
    requires std::ranges::input_range<R> &&
             std::constructible_from<M, std::ranges::range_reference_t<R>>
  void push_range(R&& rg) {
    if constexpr (std::ranges::sized_range<R>) {
      stk_back.reserve(stk_back.size() + std::ranges::size(rg));
    }
    for (auto&& x: std::forward<R>(rg)) {
      push(std::forward<decltype(x)>(x));
    }
  }

  template <typename... Args>
  const M& emplace(Args&&... args) {
    const M& x = stk_back.emplace_back(std::forward<Args>(args)...);
    cum_back = cum_back * x;
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

  friend std::string pretty(const FoldableQueue& q) {
    if constexpr (requires(const M& x) {
                    { pretty(x) } -> std::same_as<std::string>;
                  }) {
      std::string s = "[";
      for (auto i = 0uz; i < q.size(); ++i) {
        s += (i == 0 ? "" : ", ") + pretty(q[i]);
      }
      s += "]";
      return s;
    }
    return "[" + std::to_string(q.size()) + " element(s)]";
  }

 private:
  std::vector<M> stk_front, stk_back, cum_front;
  M cum_back;
};
