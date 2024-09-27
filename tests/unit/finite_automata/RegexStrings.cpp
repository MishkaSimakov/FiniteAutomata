#include "RegexStrings.h"

const auto regex_strings = [] {
  std::vector<
      std::tuple<Regex, std::vector<const char*>, std::vector<const char*>>>
      result;

  // clang-format off
  result.emplace_back(
      Regex("abcd"),
      std::vector{"abcd"},
      std::vector{"", "abc", "a", "abcde"});

  result.emplace_back(
      Regex("(a)*"),
      std::vector{"aaaaa", "a", "", "aaaaaaaaaaaaaaaaaaaaaa"},
      std::vector{"b", "aaaaaaaaaab", "ab", "ba", "bbbbb", "bbbaaa"});

  result.emplace_back(
      Regex("(a | b)*abc"),
      std::vector{"abc", "aaaaaaabc", "bbbbabc", "abababbaabbbababababc"},
      std::vector{"bc", "aaaaaaaac", "aaaaaabbc", "", "ab", "abbc", "abccc", "ccabc", "aaacaaabc"});

  result.emplace_back(
      Regex("((b | c)*a(b | c)*a)*(b | c)*"),
      std::vector{"aaaa", "abccccbbbcba", "", "bbbbcccccbbb", "bcbcbcbaaaa", "aabcbcbcbcbcbbb"},
      std::vector{"aaa", "a", "bcbcbcbcabcbcbcbc", "aadaa", "bcbcbcbcba"});

  // clang-format on

  return result;
}();
