#pragma once

#include <algorithm>
#include <concepts>
#include <ranges>
#include <utility>
#include <vector>

template <std::integral T>
std::vector<int> interval_scheduling(const std::vector<std::pair<T, T>>& I) {
  int n = I.size();
  std::vector<int> p(std::from_range, std::ranges::iota_view(0, n));
  std::ranges::sort(p, [&](int i, int j) { return I[i].second < I[j].second; });

  std::vector<int> res;
  for (int i: p) {
    if (res.empty() || I[res.back()].second <= I[i].first) {
      res.emplace_back(i);
    }
  }
  return res;
}
