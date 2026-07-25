#pragma once

#include <concepts>
#include <string>
#include <utility>

template <typename M>
concept Monoid = requires {
  { M::identity() } -> std::same_as<M>;
} && requires(const M& a, const M& b) {
  { a * b } -> std::same_as<M>;
};

template <typename M>
concept CommutativeMonoid = Monoid<M>;

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
  constexpr MonoidImpl(): x{Id()} {}
  constexpr MonoidImpl(const T& x): x{x} {}
  constexpr MonoidImpl(T&& x): x{std::move(x)} {}

  constexpr MonoidImpl operator*(const MonoidImpl& m) const {
    return MonoidImpl{Op(x, m.x)};
  }
  static constexpr MonoidImpl identity() { return MonoidImpl{Id()}; }

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
