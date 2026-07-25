#pragma once

#include <concepts>
#include <string>
#include <utility>

#include "algebra/semiring.hpp"

template <typename R>
concept Ring = Semiring<R> && requires(const R& a) {
  { -a } -> std::same_as<R>;
} && requires(const R& a, const R& b) {
  { a - b } -> std::same_as<R>;
};

template <typename T, auto Add, auto Mul, auto Zero, auto One, auto Neg>
  requires(
      requires {
        { Zero() } -> std::same_as<T>;
        { One() } -> std::same_as<T>;
      } &&
      requires(const T& a) {
        { Neg(a) } -> std::same_as<T>;
      } &&
      requires(const T& a, const T& b) {
        { Add(a, b) } -> std::same_as<T>;
        { Mul(a, b) } -> std::same_as<T>;
      })
class RingImpl {
 public:
  constexpr RingImpl(): x{Zero()} {}
  constexpr RingImpl(const T& x): x{x} {}
  constexpr RingImpl(T&& x): x{std::move(x)} {}

  constexpr RingImpl operator-() const { return RingImpl{Neg(x)}; }
  friend constexpr RingImpl operator+(const RingImpl& r, const RingImpl& s) {
    return RingImpl{Add(r.x, s.x)};
  }
  friend constexpr RingImpl operator-(const RingImpl& r, const RingImpl& s) {
    return RingImpl{Add(r.x, Neg(s.x))};
  }
  friend constexpr RingImpl operator*(const RingImpl& r, const RingImpl& s) {
    return RingImpl{Mul(r.x, s.x)};
  }
  static constexpr RingImpl zero() { return RingImpl{Zero()}; }
  static constexpr RingImpl one() { return RingImpl{One()}; }

  const T& unwrap() const { return x; }
  T& unwrap() { return x; }

  friend std::string pretty(const RingImpl& r)
    requires requires(const RingImpl& r) {
      { pretty(r.unwrap()) } -> std::same_as<std::string>;
    }
  {
    return pretty(r.unwrap());
  }

 private:
  T x;
};
