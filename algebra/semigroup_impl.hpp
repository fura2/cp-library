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
  SemigroupImpl(const T& x): x{x} {}
  SemigroupImpl(T&& x): x{std::move(x)} {}

  friend SemigroupImpl operator*(const SemigroupImpl& s,
                                 const SemigroupImpl& t) {
    return SemigroupImpl{Op(s.x, t.x)};
  }

  const T& unwrap() const { return x; }
  T& unwrap() { return x; }

  friend std::string pretty(const SemigroupImpl& s)
    requires requires(const SemigroupImpl& s) {
      { pretty(s.unwrap()) } -> std::same_as<std::string>;
    }
  {
    return pretty(s.unwrap());
  }

 private:
  T x;
};
