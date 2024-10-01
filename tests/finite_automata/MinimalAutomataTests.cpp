#include <gtest/gtest.h>

#include "FiniteAutomata.h"
#include "RegexStrings.h"

TEST(MinimalAutomataTests, test_it_accept_correct_strings_after_minimization) {
  for (const auto& [regex, correct, incorrect] : regex_strings) {
    auto automata = FiniteAutomata(regex);
    auto minimal = automata.get_minimal();

    ASSERT_LE(minimal.nodes.size(), automata.nodes.size());

    for (const auto& string : correct) {
      ASSERT_TRUE(minimal.contains(string))
          << error_message(regex, string, CORRECT);
    }

    for (const auto& string : incorrect) {
      ASSERT_FALSE(minimal.contains(string))
          << error_message(regex, string, INCORRECT);
    }
  }
}
