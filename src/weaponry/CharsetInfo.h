#pragma once
#include <ranges>

struct CharsetInfo {
  static constexpr size_t symbols_count = 26;

  static auto get_symbols() {
    return std::views::iota('a', static_cast<char>('z' + 1));
  }

  static size_t get_index(char symbol) { return symbol - 'a'; }
};

struct BinaryCharsetInfo {
  static constexpr size_t symbols_count = 2;

  static auto get_symbols() {
    // only 'a' and 'b'
    return std::views::iota('a', 'c');
  }

  static size_t get_index(char symbol) { return symbol - 'a'; }
};
