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

TEST(MinimalAutomataTests, test_it_calculate_correct_automata_for_regex) {
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

#include <fstream>

TEST(MinimalAutomataTests, test_it_calculate_correct_automata_for_dfa) {
  auto automata = FiniteAutomata();
  automata.nodes.resize(5);
  {
    auto& q0 = automata.nodes[0];
    auto& q1 = automata.nodes[1];
    auto& q2 = automata.nodes[2];
    auto& q3 = automata.nodes[3];
    auto& q4 = automata.nodes[4];

    q0.jumps = {1, 3};
    q1.jumps = {4, 2};
    q2.jumps = {4, 4};
    q3.jumps = {4, 4};
    q4.jumps = {4, 4};

    q2.is_final = true;
    q3.is_final = true;
  }

  ASSERT_TRUE(automata.is_language_equal(FiniteAutomata(Regex("b + ab"))));

  auto minimal = automata.get_minimal();

  auto expected = FiniteAutomata();
  expected.nodes.resize(4);
  {
    auto& q0 = expected.nodes[0];
    auto& q1 = expected.nodes[1];
    auto& q2 = expected.nodes[2];
    auto& q3 = expected.nodes[3];

    q0.jumps = {1, 2};
    q1.jumps = {3, 2};
    q2.jumps = {3, 3};
    q3.jumps = {3, 3};

    q2.is_final = true;
  }

  ASSERT_TRUE(expected.is_isomorphic_to(minimal));
}
