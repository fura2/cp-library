#pragma once

#include <concepts>

template <typename S>
concept Semiring =
    requires { typename S::value_type; } &&
    std::copyable<typename S::value_type> &&
    requires(const typename S::value_type& a, const typename S::value_type& b) {
      { S::add(a, b) } -> std::same_as<typename S::value_type>;
      { S::mul(a, b) } -> std::same_as<typename S::value_type>;
      { S::zero() } -> std::same_as<typename S::value_type>;
      { S::one() } -> std::same_as<typename S::value_type>;
    };

template <Semiring S>
struct AdditiveMonoidOf {
  using value_type = S::value_type;
  static constexpr value_type op(const value_type& a, const value_type& b) {
    return S::add(a, b);
  }
  static constexpr value_type id() { return S::zero(); }
};

template <Semiring S>
struct MultiplicativeMonoidOf {
  using value_type = S::value_type;
  static constexpr value_type op(const value_type& a, const value_type& b) {
    return S::mul(a, b);
  }
  static constexpr value_type id() { return S::one(); }
};
