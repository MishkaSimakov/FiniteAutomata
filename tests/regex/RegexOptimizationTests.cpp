#include <gtest/gtest.h>

#include "FiniteAutomata.h"
#include "RegexStrings.h"

// clang-format off
const inline std::vector cRegexesForOptimization = {
  Regex("aaaaaaaaaa"),
  Regex("(0 + (0 + (0)*))*(0 + 0 + 000)"),
  Regex("(0)*abab(0)*****"),
  Regex("(a + b + b)(a + (b + a))(b + (b + b))(a(b)* + a)"),
  Regex("a + aa + aaa + (a)*"),
  Regex("0"),
  Regex("(a + b)* + (a + b) + a"),
  Regex("1"),
  Regex("a"),
  Regex("aaa"),
  Regex("a + b")
};
// clang-format on

TEST(RegexOptimizationTests, test_it_accepts_same_words_after_optimization) {
  // here we use finite automata to verify that regex after optimization accepts
  // the same words as before
  for (const auto& [regex, correct, incorrect] : regex_strings) {
    auto copy = regex;
    copy.optimize();
    auto automata = FiniteAutomata(copy);

    for (const auto& string : correct) {
      ASSERT_TRUE(automata.contains(string))
          << error_message(regex, string, CORRECT);
    }

    for (const auto& string : incorrect) {
      ASSERT_FALSE(automata.contains(string))
          << error_message(regex, string, INCORRECT);
    }
  }
}

TEST(RegexOptimizationTests, test_languages_equal_after_optmization) {
  for (const Regex& regex : cRegexesForOptimization) {
    auto copy = regex;
    copy.optimize();
    auto copy_automata = FiniteAutomata(copy);
    auto original_automata = FiniteAutomata(regex);

    ASSERT_TRUE(copy_automata.is_language_equal(original_automata));
  }
}

TEST(RegexOptimizationTests, test_optimization_doesnt_make_worse) {
  for (const Regex& regex : cRegexesForOptimization) {
    auto copy = regex;
    copy.optimize();

    std::stringstream original_string;
    original_string << RegexPrinter(regex);

    std::stringstream copy_string;
    copy_string << RegexPrinter(copy);

    ASSERT_TRUE(original_string.str().size() >= copy_string.str().size());
  }
}
