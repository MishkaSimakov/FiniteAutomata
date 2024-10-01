#include <gtest/gtest.h>

#include "FiniteAutomata.h"
#include "RegexStrings.h"

TEST(ComplementTests, test_generated_regex_accepts_same_strings) {
  for (const auto& [regex, correct, incorrect] : regex_strings) {
    auto automata = FiniteAutomata(regex);
    auto generated_regex = automata.get_regex();
    automata = FiniteAutomata(generated_regex);

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
