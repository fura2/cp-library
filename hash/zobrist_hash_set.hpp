#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_set>

#include "hash/splitmix64.hpp"

template <typename T = long long, typename Hash = Splitmix64>
class ZobristHashSet {
 public:
  std::uint64_t hash() const { return h; }

  bool insert(const T& val) {
    if (data.insert(val).second) {
      h ^= hash_fn(val);
      return true;
    }
    return false;
  }

  bool erase(const T& val) {
    if (data.erase(val) > 0) {
      h ^= hash_fn(val);
      return true;
    }
    return false;
  }

  std::size_t size() const { return data.size(); }

  bool contains(const T& val) const { return data.contains(val); }

 private:
  Hash hash_fn;
  std::unordered_set<T, Hash> data;
  std::uint64_t h = 0;
};
