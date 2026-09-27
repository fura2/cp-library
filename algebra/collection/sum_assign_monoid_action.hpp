#pragma once

#include "algebra/collection/add_monoid.hpp"
#include "algebra/collection/last_monoid.hpp"
#include "algebra/monoid_action_impl.hpp"
#include "algebra/pair_monoid.hpp"

namespace sum_assign_monoid_action_detail {

template <typename T>
using ValueMonoid = PairMonoid<AddMonoid<T>, AddMonoid<int>>;

template <typename T>
using ActionMonoid = LastMonoid<T>;

template <typename T>
inline constexpr auto act =
    [](const ValueMonoid<T>& m, const ActionMonoid<T>& f) {
      return f.unwrap().has_value()
                 ? ValueMonoid<T>{m.second.unwrap() * f.unwrap().value(),
                                  m.second.unwrap()}
                 : m;
    };

}  // namespace sum_assign_monoid_action_detail

template <typename T>
using SumAssignMonoidAction =
    MonoidActionImpl<sum_assign_monoid_action_detail::ValueMonoid<T>,
                     sum_assign_monoid_action_detail::ActionMonoid<T>,
                     sum_assign_monoid_action_detail::act<T>>;
