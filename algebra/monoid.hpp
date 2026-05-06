#pragma once

#include <concepts>

template <typename M>
concept Monoid =
    requires { typename M::value_type; } &&
    requires(const typename M::value_type& a, const typename M::value_type& b) {
      { M::op(a, b) } -> std::same_as<typename M::value_type>;
      { M::id() } -> std::same_as<typename M::value_type>;
    };

// template <typename T>
// struct AddMonoid {
//   using value_type = T;
//   static value_type op(const value_type& a, const value_type& b) {
//     return a + b;
//   }
//   static value_type id() { return {}; }
// };
