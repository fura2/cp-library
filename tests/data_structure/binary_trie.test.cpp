#include "data_structure/binary_trie.hpp"

#include <cassert>

int main() {
  const BinaryTrieNode node;
  assert(node.size() == 0 && node.zero() == nullptr && node.one() == nullptr);
  BinaryTrie trie;
  assert(trie.zero() == '0' && trie.one() == '1');
  assert(trie.root()->size() == 0);
  trie.add("");
  trie.add("0");
  trie.add("01");
  trie.add("01");
  trie.add("10");
  assert(trie.root()->size() == 5);
  assert(trie.root()->zero()->size() == 3);
  assert(trie.root()->zero()->one()->size() == 2);
  assert(trie.root()->one()->zero()->size() == 1);
  trie.erase("01");
  assert(trie.root()->zero()->one()->size() == 1);
  trie.erase("");
  trie.erase("0");
  trie.erase("01");
  trie.erase("10");
  assert(trie.root()->size() == 0 && trie.root()->zero()->size() == 0);
  trie.add("01");
  assert(trie.root()->zero()->one()->size() == 1);

  BinaryTrie custom{'a', 'b'};
  custom.add("abba");
  assert(custom.zero() == 'a' && custom.one() == 'b');
  assert(custom.root()->zero()->one()->one()->zero()->size() == 1);
  custom.erase("abba");
  assert(custom.root()->size() == 0);
}
