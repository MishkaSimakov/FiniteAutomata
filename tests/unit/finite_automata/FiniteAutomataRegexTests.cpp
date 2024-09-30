#include <gtest/gtest.h>

#include "FiniteAutomata.h"
#include "RegexStrings.h"

TEST(FiniteAutomataRegexTests, test_it_accepts_correct_words) {
  for (const auto& [regex, correct, incorrect] : regex_strings) {
    auto automata = FiniteAutomata(regex);

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
