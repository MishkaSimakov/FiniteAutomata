#include <gtest/gtest.h>

#include "FiniteAutomata.h"
#include "RegexStrings.h"

TEST(ComplementTests, test_complement_accepts_opposite_strings) {
  for (const auto& [regex, correct, incorrect] : regex_strings) {
    auto automata = FiniteAutomata(regex).complement();

    for (const auto& string : correct) {
      ASSERT_FALSE(automata.contains(string));
    }

    for (const auto& string : incorrect) {
      ASSERT_TRUE(automata.contains(string));
    }
  }
}

TEST(ComplementTests, test_complement_language_equality) {
  {
    auto even_a = FiniteAutomata(Regex("((b)*a(b)*a)*(b)*"));
    even_a.complement();

    auto odd_a = FiniteAutomata(Regex("(b)*a((b)*a(b)*a)*(b)*"));

    ASSERT_TRUE(even_a.is_language_equal(odd_a));
  }

  {
    auto ab_equal_ba =
        FiniteAutomata(Regex("(a(a)*b(b)*)*a(a)* + (b(b)*a(a)*)*b(b)* + 1"));
    ab_equal_ba.complement();

    auto ab_not_equal_ba = FiniteAutomata(
        Regex("(a(a)*b(b)*)(a(a)*b(b)*)* + (b(b)*a(a)*)(b(b)*a(a)*)*"));

    ASSERT_TRUE(ab_equal_ba.is_language_equal(ab_not_equal_ba));
  }
}
