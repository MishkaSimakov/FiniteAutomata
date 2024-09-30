#include "RegexStrings.h"

const auto regex_strings = [] {
  std::vector<
      std::tuple<Regex, std::vector<const char*>, std::vector<const char*>>>
      result;

  // clang-format off
  result.emplace_back(
      Regex("abab"),
      std::vector{"abab"},
      std::vector{"", "ab", "a", "abbbb", "abbb", "abaa"});

  result.emplace_back(
      Regex("(a)*"),
      std::vector{"aaaaa", "a", "", "aaaaaaaaaaaaaaaaaaaaaa"},
      std::vector{"b", "aaaaaaaaaab", "ab", "ba", "bbbbb", "bbbaaa"});

  result.emplace_back(
      Regex("(a + b)*ab"),
      std::vector{"ab", "aaaaaaab", "bbbbab", "abababbaabbbabababab"},
      std::vector{"bb", "aaaaaaaabb", "aaaaaabba", "", "a", "b", "abb", "aba", "aaa"});

  // clang-format on

  return result;
}();
