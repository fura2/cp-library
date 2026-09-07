#pragma once

#include <concepts>
#include <string>
#include <utility>

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
  RingImpl(): x{Zero()} {}
  RingImpl(const T& x): x{x} {}
  RingImpl(T&& x): x{std::move(x)} {}

  RingImpl operator-() const { return RingImpl{Neg(x)}; }
  friend RingImpl operator+(const RingImpl& r, const RingImpl& s) {
    return RingImpl{Add(r.x, s.x)};
  }
  friend RingImpl operator-(const RingImpl& r, const RingImpl& s) {
    return RingImpl{Add(r.x, Neg(s.x))};
  }
  friend RingImpl operator*(const RingImpl& r, const RingImpl& s) {
    return RingImpl{Mul(r.x, s.x)};
  }
  static RingImpl zero() { return RingImpl{Zero()}; }
  static RingImpl one() { return RingImpl{One()}; }

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
