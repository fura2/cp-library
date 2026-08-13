#pragma once

#include <concepts>
#include <string>
#include <utility>

template <typename T, auto Add, auto Mul, auto Zero, auto One>
  requires(
      requires {
        { Zero() } -> std::same_as<T>;
        { One() } -> std::same_as<T>;
      } &&
      requires(const T& a, const T& b) {
        { Add(a, b) } -> std::same_as<T>;
        { Mul(a, b) } -> std::same_as<T>;
      })
class SemiringImpl {
 public:
  constexpr SemiringImpl(): x{Zero()} {}
  constexpr SemiringImpl(const T& x): x{x} {}
  constexpr SemiringImpl(T&& x): x{std::move(x)} {}

  friend constexpr SemiringImpl operator+(const SemiringImpl& s,
                                          const SemiringImpl& t) {
    return SemiringImpl{Add(s.x, t.x)};
  }
  friend constexpr SemiringImpl operator*(const SemiringImpl& s,
                                          const SemiringImpl& t) {
    return SemiringImpl{Mul(s.x, t.x)};
  }
  static constexpr SemiringImpl zero() { return SemiringImpl{Zero()}; }
  static constexpr SemiringImpl one() { return SemiringImpl{One()}; }

  const T& unwrap() const { return x; }
  T& unwrap() { return x; }

  friend std::string pretty(const SemiringImpl& s)
    requires requires(const SemiringImpl& s) {
      { pretty(s.unwrap()) } -> std::same_as<std::string>;
    }
  {
    return pretty(s.unwrap());
  }

 private:
  T x;
};
