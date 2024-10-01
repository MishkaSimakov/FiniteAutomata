#include <gtest/gtest.h>

#include "FiniteAutomata.h"
#include "FiniteAutomataPrinter.h"
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

TEST(MinimalAutomataTests,
     test_it_calculate_correct_automata_for_example_from_lecture) {
  auto regex = Regex("(a + b(a + bb)*ba)(a + b)*");

  auto predicted = FiniteAutomata();
  predicted.nodes.resize(3);

  auto& q0 = predicted.nodes[0];
  auto& q1 = predicted.nodes[1];
  auto& q2 = predicted.nodes[2];

  q0.jumps = {2, 1};
  q1.jumps = {1, 0};
  q2.jumps = {2, 2};
  q2.is_final = true;

  auto actual = FiniteAutomata(regex).get_minimal();

  ASSERT_TRUE(actual.is_isomorphic_to(predicted));
}
