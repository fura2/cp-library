#pragma once

#include <concepts>
#include <string>
#include <utility>

template <typename T, auto Op>
  requires requires(const T& a, const T& b) {
    { Op(a, b) } -> std::same_as<T>;
  }
class SemigroupImpl {
 public:
  constexpr SemigroupImpl(const T& x): x{x} {}
  constexpr SemigroupImpl(T&& x): x{std::move(x)} {}

  friend constexpr SemigroupImpl operator*(const SemigroupImpl& m,
                                           const SemigroupImpl& n) {
    return SemigroupImpl{Op(m.x, n.x)};
  }

  const T& unwrap() const { return x; }
  T& unwrap() { return x; }

  friend std::string pretty(const SemigroupImpl& m)
    requires requires(const SemigroupImpl& m) {
      { pretty(m.unwrap()) } -> std::same_as<std::string>;
    }
  {
    return pretty(m.unwrap());
  }

 private:
  T x;
};
