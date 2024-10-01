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

  result.emplace_back(
      Regex("a(a)*"),
      std::vector{"a", "aa", "aaa", "aaaaaaaaaaaaaaaaaaaaaaaa"},
      std::vector{"", "b", "ab", "ba", "bbbbbbbbbb", "aaaaaaaaaab"});

  result.emplace_back(
        Regex("0"),
        std::vector<const char*>{},
        std::vector{"", "a", "b", "aa", "ab", "ba", "bb", "abababababbab"});

  result.emplace_back(
        Regex("1"),
        std::vector{""},
        std::vector{"a", "b", "aa", "ab", "ba", "bb", "abababababbab"});

  result.emplace_back(
          Regex("1a1111a(1 + a)"),
          std::vector{"aa", "aaa"},
          std::vector{"", "ab", "ba", "baa", "aab", "aba", "aaaaaa", "aaaa"});

  result.emplace_back(
          Regex("aaaaa0aa"),
          std::vector<const char*>{},
          std::vector{"", "a", "b", "aa", "ab", "ba", "bb", "abababababbab"});

  result.emplace_back(
          Regex("(a(a)*b(b)*)*a(a)* + (b(b)*a(a)*)*b(b)* + 1"),
          std::vector{"", "ababa", "aaaaaba", "abba", "baab", "bbbbaaabbbb", "babab", "a", "b", "aaaaaa", "bbbbbbb", "aa", "bb"},
          std::vector{"ba", "ab", "abab", "baba", "baaaaababbbba", "abbbbab"});


  // clang-format on

  return result;
}();
