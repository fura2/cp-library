#pragma once

#include <string>

struct Edge {
  int from, to;
  int id;
  Edge(int from, int to, int id = -1): from{from}, to{to}, id{id} {}

  friend std::string pretty(const Edge& e) {
    return "(" + std::to_string(e.from) + ", " + std::to_string(e.to) + ")";
  }
};
