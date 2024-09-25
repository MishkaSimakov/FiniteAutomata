#include <gtest/gtest.h>

#include "FiniteAutomata.h"
#include "regex/RegexParser.h"

TEST(FiniteAutomataRegexTests,
     TestItBuildAutomataForRegexAndAcceptCorrectWords) {
  {
    auto automata = FiniteAutomata(*RegexParser::parse("abcd"));

    ASSERT_TRUE(automata.containts_word("abcd"));
    ASSERT_FALSE(automata.containts_word(""));
    ASSERT_FALSE(automata.containts_word("abc"));
    ASSERT_FALSE(automata.containts_word("a"));
    ASSERT_FALSE(automata.containts_word("abcde"));
  }

  {
    auto automata = FiniteAutomata(*RegexParser::parse("(a)*"));

    ASSERT_TRUE(automata.containts_word("aaaaaaa"));
    ASSERT_TRUE(automata.containts_word(""));
    ASSERT_TRUE(automata.containts_word("a"));
    ASSERT_TRUE(automata.containts_word("aaa"));
    ASSERT_FALSE(automata.containts_word("baaaa"));
    ASSERT_FALSE(automata.containts_word("b"));
    ASSERT_FALSE(automata.containts_word("aaaaab"));
    ASSERT_FALSE(automata.containts_word("bbbbbb"));
  }

  {
    auto automata = FiniteAutomata(*RegexParser::parse("(a | b)*abc"));

    ASSERT_TRUE(automata.containts_word("abababaaaaabc"));
    ASSERT_TRUE(automata.containts_word("abc"));
    ASSERT_TRUE(automata.containts_word("bbbbbbabc"));
    ASSERT_TRUE(automata.containts_word("aaaaaaabc"));
    ASSERT_FALSE(automata.containts_word("abababababab"));
    ASSERT_FALSE(automata.containts_word(""));
    ASSERT_FALSE(automata.containts_word("abbc"));
    ASSERT_FALSE(automata.containts_word("bc"));
    ASSERT_FALSE(automata.containts_word("abca"));
  }

  {
    // even count of a
    auto automata =
        FiniteAutomata(*RegexParser::parse("((b | c)*a(b | c)*a)*(b | c)*"));

    ASSERT_TRUE(automata.containts_word("aaaa"));
    ASSERT_TRUE(automata.containts_word("abccccbbbcba"));
    ASSERT_TRUE(automata.containts_word(""));
    ASSERT_TRUE(automata.containts_word("bbbbcccccbbb"));
    ASSERT_TRUE(automata.containts_word("bcbcbcbaaaa"));
    ASSERT_TRUE(automata.containts_word("aabcbcbcbcbcbbb"));

    ASSERT_FALSE(automata.containts_word("aaa"));
    ASSERT_FALSE(automata.containts_word("a"));
    ASSERT_FALSE(automata.containts_word("bcbcbcbcabcbcbcbc"));
    ASSERT_FALSE(automata.containts_word("aadaa"));
    ASSERT_FALSE(automata.containts_word("bcbcbcbcba"));
  }
}
