#pragma once

struct Edge {
  int from, to;
  int id;
  Edge(int from, int to, int id = -1): from{from}, to{to}, id{id} {}
};
