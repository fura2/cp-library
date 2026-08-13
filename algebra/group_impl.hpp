#pragma once

#include <concepts>
#include <string>
#include <utility>

template <typename T, auto Op, auto Id, auto Inv>
  requires(
      requires {
        { Id() } -> std::same_as<T>;
      } &&
      requires(const T& a) {
        { Inv(a) } -> std::same_as<T>;
      } &&
      requires(const T& a, const T& b) {
        { Op(a, b) } -> std::same_as<T>;
      })
class GroupImpl {
 public:
  constexpr GroupImpl(): x{Id()} {}
  constexpr GroupImpl(const T& x): x{x} {}
  constexpr GroupImpl(T&& x): x{std::move(x)} {}

  friend constexpr GroupImpl operator*(const GroupImpl& g, const GroupImpl& h) {
    return GroupImpl{Op(g.x, h.x)};
  }
  static constexpr GroupImpl identity() { return GroupImpl{Id()}; }

  constexpr GroupImpl inverse() const { return GroupImpl{Inv(x)}; }

  const T& unwrap() const { return x; }
  T& unwrap() { return x; }

  friend std::string pretty(const GroupImpl& g)
    requires requires(const GroupImpl& g) {
      { pretty(g.unwrap()) } -> std::same_as<std::string>;
    }
  {
    return pretty(g.unwrap());
  }

 private:
  T x;
};
