#pragma once

#include <concepts>
#include <ranges>

template <typename GraphT>
concept IncidenceGraph = requires {
  typename GraphT::edge_type;
  { GraphT::directed } -> std::convertible_to<bool>;
} && requires(const GraphT& G) {
  { G.num_vertices() } -> std::same_as<int>;
} && requires(const GraphT& G, int u) {
  { G[u] } -> std::ranges::input_range;
  requires std::same_as<std::ranges::range_reference_t<decltype(G[u])>,
                        const typename GraphT::edge_type&>;
};

template <typename GraphT>
concept Undirected = !GraphT::directed;
template <typename GraphT>
concept Directed = GraphT::directed;
