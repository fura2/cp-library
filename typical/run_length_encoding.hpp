#pragma once

#include <ranges>
#include <utility>
#include <vector>

template <std::ranges::input_range R>
auto run_length_encoding(R&& rg) {
  std::vector<std::pair<std::ranges::range_value_t<R>, int>> rle;
  for (const auto& e: rg) {
    if (rle.empty() || rle.back().first != e) {
      rle.emplace_back(e, 1);
    }
    else {
      ++rle.back().second;
    }
  }
  return rle;
}
