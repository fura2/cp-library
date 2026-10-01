#pragma once

#include "algebra/collection/add_monoid.hpp"
#include "algebra/monoid_action_impl.hpp"
#include "algebra/pair_monoid.hpp"

namespace sum_add_monoid_action_detail {

template <typename T>
using ValueMonoid = PairMonoid<AddMonoid<T>, AddMonoid<int>>;

template <typename T>
using ActionMonoid = AddMonoid<T>;

template <typename T>
inline constexpr auto act =
    [](const ValueMonoid<T>& x, const ActionMonoid<T>& f) {
      return ValueMonoid<T>{x.first.unwrap() + x.second.unwrap() * f.unwrap(),
                            x.second.unwrap()};
    };

}  // namespace sum_add_monoid_action_detail

template <typename T>
using SumAddMonoidAction =
    MonoidActionImpl<sum_add_monoid_action_detail::ValueMonoid<T>,
                     sum_add_monoid_action_detail::ActionMonoid<T>,
                     sum_add_monoid_action_detail::act<T>>;
