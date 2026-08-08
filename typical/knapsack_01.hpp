#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <iterator>
#include <limits>
#include <numeric>
#include <utility>
#include <vector>

namespace knapsack_01_detail {

template <std::signed_integral T, std::signed_integral U>
U knapsack_01_weight_dp(const std::vector<T>& w, const std::vector<U>& v, T W) {
  int n = w.size();
  std::vector<U> dp(W + 1, std::numeric_limits<U>::min());
  dp[0] = 0;
  for (int i = 0; i < n; ++i) {
    for (T x = W - w[i]; x >= 0; --x) {
      if (dp[x] >= 0 && dp[x + w[i]] < dp[x] + v[i]) {
        dp[x + w[i]] = dp[x] + v[i];
      }
    }
  }
  return std::ranges::max(dp);
}

template <std::signed_integral T, std::signed_integral U>
U knapsack_01_value_dp(const std::vector<T>& w, const std::vector<U>& v, T W) {
  int n = w.size();
  U vsum = std::reduce(v.begin(), v.end());
  std::vector<T> dp(vsum + 1, std::numeric_limits<T>::max());
  dp[0] = 0;
  for (int i = 0; i < n; ++i) {
    for (U x = vsum - v[i]; x >= 0; --x) {
      if (dp[x] <= W - w[i] && dp[x + v[i]] > dp[x] + w[i]) {
        dp[x + v[i]] = dp[x] + w[i];
      }
    }
  }
  U res = vsum;
  while (dp[res] > W) --res;
  return res;
}

template <std::signed_integral T, std::signed_integral U>
U knapsack_01_meet_in_the_middle(const std::vector<T>& w,
                                 const std::vector<U>& v,
                                 T W) {
  int n = w.size();
  int nl = n / 2, nr = (n + 1) / 2;
  std::vector<std::pair<T, U>> L;
  L.reserve(1 << nl);
  for (int S = 0; S < (1 << nl); ++S) {
    T wsum = 0;
    U vsum = 0;
    for (int i = 0; i < nl; ++i) {
      if ((S >> i) & 1) {
        wsum += w[i];
        vsum += v[i];
      }
    }
    if (wsum <= W) L.emplace_back(wsum, vsum);
  }
  std::ranges::sort(L, [](const auto& a, const auto& b) {
    if (a.first != b.first) return a.first < b.first;
    return a.second > b.second;
  });
  {
    int m = L.size();
    std::vector<std::pair<T, U>> L2;
    L2.reserve(m);
    for (int i = 0; i < m; ++i) {
      if (!L2.empty() && L2.back().second >= L[i].second) continue;
      L2.emplace_back(L[i]);
    }
    L = std::move(L2);
  }

  U res = 0;
  for (int S = 0; S < (1 << nr); ++S) {
    T wsum = 0;
    U vsum = 0;
    for (int i = 0; i < nr; ++i) {
      if ((S >> i) & 1) {
        wsum += w[nl + i];
        vsum += v[nl + i];
      }
    }
    if (wsum > W) continue;
    U lvsum = std::prev(std::ranges::upper_bound(
                            L, W - wsum, {}, &std::pair<T, U>::first))
                  ->second;
    if (res < lvsum + vsum) {
      res = lvsum + vsum;
    }
  }
  return res;
}

}  // namespace knapsack_01_detail

enum class KnapsackAlgorithm { WeightDp, ValueDp, MeetInTheMiddle };

template <std::signed_integral T, std::signed_integral U>
U knapsack_01(const std::vector<T>& w,
              const std::vector<U>& v,
              T W,
              KnapsackAlgorithm algo) {
  assert(w.size() == v.size());
  assert(W >= 0);

  auto solve = [algo](
                   const std::vector<T>& w, const std::vector<U>& v, T W) -> U {
    switch (algo) {
      case KnapsackAlgorithm::WeightDp:
        return knapsack_01_detail::knapsack_01_weight_dp(w, v, W);
      case KnapsackAlgorithm::ValueDp:
        return knapsack_01_detail::knapsack_01_value_dp(w, v, W);
      case KnapsackAlgorithm::MeetInTheMiddle:
        return knapsack_01_detail::knapsack_01_meet_in_the_middle(w, v, W);
    }
    std::unreachable();
  };

  int n = w.size();
  bool normal = true;
  for (int i = 0; i < n; ++i) {
    if (w[i] <= 0 || w[i] > W || v[i] <= 0) normal = false;
  }
  if (normal) {
    return solve(w, v, W);
  }
  else {
    U vsum = 0;
    T W2 = W;
    std::vector<T> w2;
    std::vector<U> v2;
    w2.reserve(n);
    v2.reserve(n);
    for (int i = 0; i < n; ++i) {
      if (w[i] >= 0 && v[i] <= 0) {
        // nop
      }
      else if (w[i] <= 0 && v[i] >= 0) {
        vsum += v[i];
        W2 -= w[i];
      }
      else if (w[i] < 0 && v[i] < 0) {
        vsum += v[i];
        W2 -= w[i];
        w2.emplace_back(-w[i]);
        v2.emplace_back(-v[i]);
      }
      else {
        w2.emplace_back(w[i]);
        v2.emplace_back(v[i]);
      }
    }
    return vsum + solve(w2, v2, W2);
  }
}
