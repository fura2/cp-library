#pragma once

#include <concepts>

template <typename S>
concept Semiring = requires {
  { S::zero() } -> std::same_as<S>;
  { S::one() } -> std::same_as<S>;
} && requires(const S& a, const S& b) {
  { a + b } -> std::same_as<S>;
  { a * b } -> std::same_as<S>;
} && requires(S a, const S& b) {
  { a += b } -> std::same_as<S&>;
  { a *= b } -> std::same_as<S&>;
};

template <Semiring S>
class AdditiveMonoidOfSemiring {
 public:
  AdditiveMonoidOfSemiring(): x{S::zero()} {}
  AdditiveMonoidOfSemiring(const S& x): x{x} {}

  AdditiveMonoidOfSemiring& operator*=(const AdditiveMonoidOfSemiring& a) {
    x += a.x;
    return *this;
  }
  friend AdditiveMonoidOfSemiring operator*(const AdditiveMonoidOfSemiring& a,
                                            const AdditiveMonoidOfSemiring& b) {
    return AdditiveMonoidOfSemiring{a} *= b;
  }
  static AdditiveMonoidOfSemiring identity() { return S::zero(); }

  const S& unwrap() const { return x; }
  S& unwrap() { return x; }

 private:
  S x;
};

template <Semiring S>
class MultiplicativeMonoidOfSemiring {
 public:
  MultiplicativeMonoidOfSemiring(): x{S::one()} {}
  MultiplicativeMonoidOfSemiring(const S& x): x{x} {}

  MultiplicativeMonoidOfSemiring& operator*=(
      const MultiplicativeMonoidOfSemiring& a) {
    x *= a.x;
    return *this;
  }
  friend MultiplicativeMonoidOfSemiring operator*(
      const MultiplicativeMonoidOfSemiring& a,
      const MultiplicativeMonoidOfSemiring& b) {
    return MultiplicativeMonoidOfSemiring{a} *= b;
  }
  static MultiplicativeMonoidOfSemiring identity() { return S::one(); }

  const S& unwrap() const { return x; }
  S& unwrap() { return x; }

 private:
  S x;
};
