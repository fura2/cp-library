#pragma once

#include <concepts>

template <typename R>
concept Semiring =
    requires { typename R::value_type; } &&
    requires(const typename R::value_type& a, const typename R::value_type& b) {
      { R::add(a, b) } -> std::same_as<typename R::value_type>;
      { R::mul(a, b) } -> std::same_as<typename R::value_type>;
      { R::zero() } -> std::same_as<typename R::value_type>;
      { R::one() } -> std::same_as<typename R::value_type>;
    };

template <Semiring R>
struct AdditiveMonoidOfSemiring {
  using value_type = R::value_type;
  static constexpr value_type op(const value_type& a, const value_type& b) {
    return R::add(a, b);
  }
  static constexpr value_type id() { return R::zero(); }
};

template <Semiring R>
struct MultiplicativeMonoidOfSemiring {
  using value_type = R::value_type;
  static constexpr value_type op(const value_type& a, const value_type& b) {
    return R::mul(a, b);
  }
  static constexpr value_type id() { return R::one(); }
};
