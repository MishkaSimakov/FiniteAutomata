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

TEST(ComplementTests, test_complement_for_big_regexes) {
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

TEST(ComplementTests, test_complement_for_nothing_is_everything) {
  auto nothing = FiniteAutomata(Regex("0"));
  auto everything = FiniteAutomata(Regex("(a + b)*"));

  // complement to nothing is everything
  auto copy = nothing;
  copy.complement();
  ASSERT_TRUE(copy.is_language_equal(everything));

  // complement to everything is nothing
  copy = everything;
  copy.complement();
  ASSERT_TRUE(copy.is_language_equal(nothing));
}

TEST(CompelementTests, test_complement_for_basic_regexes) {
  {
    auto only_a = FiniteAutomata(Regex("(a)*"));
    auto contains_b = FiniteAutomata(Regex("(a + b)*b(a + b)*"));

    ASSERT_TRUE(only_a.complement().is_language_equal(contains_b));
  }

  {
    auto length_le_one = FiniteAutomata(Regex("1 + a + b"));
    auto length_gt_one = FiniteAutomata(Regex("(a + b)(a + b)(a + b)*"));

    ASSERT_TRUE(length_gt_one.complement().is_language_equal(length_le_one));
  }
}
