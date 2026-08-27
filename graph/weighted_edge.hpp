#pragma once

#include <concepts>
#include <string>

template <typename T>
struct WeightedEdge {
  int from, to;
  T cost;
  int id;
  WeightedEdge(int from, int to, const T& cost, int id = -1)
      : from{from}, to{to}, cost{cost}, id{id} {}

  friend std::string pretty(const WeightedEdge& e)
    requires requires(const T& c) {
      { pretty(c) } -> std::same_as<std::string>;
    }
  {
    return "(" + std::to_string(e.from) + ", " + std::to_string(e.to) + "; " +
           pretty(e.cost) + ")";
  }
};
