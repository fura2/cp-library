#pragma once

#include <concepts>
#include <string>
#include <utility>

template <typename S>
concept Semiring = requires {
  { S::zero() } -> std::same_as<S>;
  { S::one() } -> std::same_as<S>;
} && requires(const S& a, const S& b) {
  { a + b } -> std::same_as<S>;
  { a * b } -> std::same_as<S>;
};

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

  constexpr SemiringImpl operator+(const SemiringImpl& s) const {
    return SemiringImpl{Add(x, s.x)};
  }
  constexpr SemiringImpl operator*(const SemiringImpl& s) const {
    return SemiringImpl{Mul(x, s.x)};
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

/*
template <Semiring S>
class AdditiveMonoid {
 public:
  AdditiveMonoid(): x{S::zero()} {}
  AdditiveMonoid(const S& x): x{x} {}

  AdditiveMonoid& operator*=(const AdditiveMonoid& a) {
    x += a.x;
    return *this;
  }
  friend AdditiveMonoid operator*(const AdditiveMonoid& a,
                                  const AdditiveMonoid& b) {
    return AdditiveMonoid{a} *= b;
  }
  static AdditiveMonoid identity() { return S::zero(); }

  const S& unwrap() const { return x; }
  S& unwrap() { return x; }

 private:
  S x;
};

template <Semiring S>
class MultiplicativeMonoid {
 public:
  MultiplicativeMonoid(): x{S::one()} {}
  MultiplicativeMonoid(const S& x): x{x} {}

  MultiplicativeMonoid& operator*=(const MultiplicativeMonoid& a) {
    x *= a.x;
    return *this;
  }
  friend MultiplicativeMonoid operator*(const MultiplicativeMonoid& a,
                                        const MultiplicativeMonoid& b) {
    return MultiplicativeMonoid{a} *= b;
  }
  static MultiplicativeMonoid identity() { return S::one(); }

  const S& unwrap() const { return x; }
  S& unwrap() { return x; }

 private:
  S x;
};
*/
