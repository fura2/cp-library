#pragma once

#include <concepts>
#include <string>
#include <utility>

template <typename T, auto Op, auto Id>
  requires(
      requires {
        { Id() } -> std::same_as<T>;
      } &&
      requires(const T& a, const T& b) {
        { Op(a, b) } -> std::same_as<T>;
      })
class MonoidImpl {
 public:
  MonoidImpl(): x{Id()} {}
  MonoidImpl(const T& x): x{x} {}
  MonoidImpl(T&& x): x{std::move(x)} {}

  friend MonoidImpl operator*(const MonoidImpl& m, const MonoidImpl& n) {
    return MonoidImpl{Op(m.x, n.x)};
  }
  static MonoidImpl identity() { return MonoidImpl{Id()}; }

  const T& unwrap() const { return x; }
  T& unwrap() { return x; }

  friend std::string pretty(const MonoidImpl& m)
    requires requires(const MonoidImpl& m) {
      { pretty(m.unwrap()) } -> std::same_as<std::string>;
    }
  {
    return pretty(m.unwrap());
  }

 private:
  T x;
};
