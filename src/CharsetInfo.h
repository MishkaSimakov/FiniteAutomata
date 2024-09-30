#pragma once
#include <ranges>

// a ... z
struct CharsetInfo {
  static constexpr size_t symbols_count = 26;

  static auto get_symbols() {
    return std::views::iota('a', static_cast<char>('z' + 1));
  }

  static size_t get_index(char symbol) { return symbol - 'a'; }

  static bool contains(char symbol) { return 'a' <= symbol && symbol <= 'z'; }
};

// only 'a' and 'b'
struct BinaryCharsetInfo {
  static constexpr size_t symbols_count = 2;

  static auto get_symbols() { return std::views::iota('a', 'c'); }

  static size_t get_index(char symbol) { return symbol - 'a'; }

  static bool contains(char symbol) { return symbol == 'a' || symbol == 'b'; }
};

using Charset = BinaryCharsetInfo;
