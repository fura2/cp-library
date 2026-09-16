#pragma once

#include <cassert>
#include <string>

class BinaryTrieNode {
 public:
  BinaryTrieNode(): sz{0}, l{nullptr}, r{nullptr} {}

  int size() const { return sz; }
  const BinaryTrieNode* zero() const { return l; }
  const BinaryTrieNode* one() const { return r; }

 private:
  int sz;
  BinaryTrieNode* l;
  BinaryTrieNode* r;

  friend class BinaryTrie;
};

class BinaryTrie {
 public:
  explicit BinaryTrie(char zero = '0', char one = '1')
      : c0{zero}, c1{one}, r{new BinaryTrieNode{}} {
    assert(c0 != c1);
  }

  const BinaryTrieNode* root() const { return r; }
  char zero() const { return c0; }
  char one() const { return c1; }

  void add(const std::string& s) {
    BinaryTrieNode* p = r;
    p->sz++;
    for (const auto& c: s) {
      if (c == c0) {
        if (!p->l) p->l = new BinaryTrieNode{};
        p = p->l;
      }
      else if (c == c1) {
        if (!p->r) p->r = new BinaryTrieNode{};
        p = p->r;
      }
      else {
        assert(false);
      }
      p->sz++;
    }
  }

  void erase(const std::string& s) {
    BinaryTrieNode* p = r;
    p->sz--;
    for (const auto& c: s) {
      if (c == c0) {
        p = p->l;
      }
      else if (c == c1) {
        p = p->r;
      }
      else {
        assert(false);
      }
      p->sz--;
    }
  }

 private:
  char c0, c1;
  BinaryTrieNode* r;
};
