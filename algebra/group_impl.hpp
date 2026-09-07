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
  GroupImpl(): x{Id()} {}
  GroupImpl(const T& x): x{x} {}
  GroupImpl(T&& x): x{std::move(x)} {}

  friend GroupImpl operator*(const GroupImpl& g, const GroupImpl& h) {
    return GroupImpl{Op(g.x, h.x)};
  }
  static GroupImpl identity() { return GroupImpl{Id()}; }

  GroupImpl inverse() const { return GroupImpl{Inv(x)}; }

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
