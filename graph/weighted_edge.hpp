#pragma once

template <typename T>
struct WeightedEdge {
  int from, to;
  T cost;
  int id;
  WeightedEdge(int from, int to, const T& cost, int id = -1)
      : from{from}, to{to}, cost{cost}, id{id} {}
};
