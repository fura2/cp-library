#pragma once

#include "algebra/collection/add_monoid.hpp"
#include "algebra/collection/max_monoid.hpp"
#include "algebra/monoid_action_impl.hpp"

namespace max_add_monoid_action_detail {

template <typename T>
using ValueMonoid = MaxMonoid<T>;

template <typename T>
using ActionMonoid = AddMonoid<T>;

template <typename T>
inline constexpr auto act =
    [](const ValueMonoid<T>& x, const ActionMonoid<T>& f) {
      if (x.unwrap() == ValueMonoid<T>::identity().unwrap()) {
        return x;
      }
      return ValueMonoid<T>{x.unwrap() + f.unwrap()};
    };

}  // namespace max_add_monoid_action_detail

template <typename T>
using MaxAddMonoidAction =
    MonoidActionImpl<max_add_monoid_action_detail::ValueMonoid<T>,
                     max_add_monoid_action_detail::ActionMonoid<T>,
                     max_add_monoid_action_detail::act<T>>;
