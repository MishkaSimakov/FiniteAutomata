#include <gtest/gtest.h>

#include "RegexStrings.h"
#include "NonDeterministicFiniteAutomata.h"

TEST(NonDeterministicTests, test_it_returns_correct_final_nodes) {
  auto automata = NonDeterministicFiniteAutomata();

  auto& q0 = automata.get_start_node();
  auto& q1 = automata.add_node();
  auto& q2 = automata.add_node();
  auto& q3 = automata.add_node();
  auto& q4 = automata.add_node();
  auto& q5 = automata.add_node();

  q0.is_final = true;
  q3.is_final = true;
  q5.is_final = true;

  using NodesStorageT =
      std::unordered_set<NonDeterministicFiniteAutomata::Node*>;
  NodesStorageT final_nodes;
  for (auto& node : automata.get_final_nodes()) {
    final_nodes.insert(&node);
  }

  NodesStorageT expected = {&q0, &q3, &q5};
  ASSERT_EQ(expected, final_nodes);
}

TEST(NonDeterministicTests, test_it_removes_empty_jumps_correctly) {
  constexpr char cEmptyChar = NonDeterministicFiniteAutomata::cEmptyChar;

  auto automata = NonDeterministicFiniteAutomata();

  auto& q0 = automata.get_start_node();
  auto& q1 = automata.add_node();
  auto& q2 = automata.add_node();
  auto& q3 = automata.add_node();
  auto& q4 = automata.add_node();
  auto& q5 = automata.add_node();

  q0.jumps.emplace(cEmptyChar, &q1);
  q0.jumps.emplace('a', &q2);
  q0.jumps.emplace('b', &q4);

  q1.jumps.emplace(cEmptyChar, &q3);

  q2.jumps.emplace(cEmptyChar, &q3);

  q3.jumps.emplace('a', &q5);

  //
  q3.is_final = true;

  automata.remove_empty_jumps();

  auto& new_q0 = automata.get_start_node();

  ASSERT_TRUE(new_q0.is_final);
  ASSERT_TRUE(new_q0.jumps.size() == 3);
  ASSERT_TRUE(new_q0.jumps.count('b') == 1 && new_q0.jumps.count('a') == 2);

  ASSERT_TRUE(new_q0.jumps.find('b')->second->jumps.empty());

  // there are two nodes reachable from new_q0 by 'a'
  // one of them must be final and another not final
  auto* new_q1 = new_q0.jumps.equal_range('a').first->second;
  auto* new_q2 = std::next(new_q0.jumps.equal_range('a').first)->second;

  ASSERT_TRUE(new_q1->is_final != new_q2->is_final);

  if (new_q1->is_final) {
    std::swap(new_q1, new_q2);
  }

  ASSERT_TRUE(new_q1->jumps.empty());
  ASSERT_TRUE(new_q2->jumps.size() == 1 && new_q2->jumps.contains('a') &&
              new_q2->jumps.find('a')->second == new_q1);
}

TEST(NonDeterministicTests,
     test_it_gets_regex_correctly_when_built_from_regex) {
  // we do regex -> automata -> regex -> automata and expect that last automata
  // accepts the same words as first regex

  for (const auto& [regex, correct, incorrect] : regex_strings) {
    auto automata = NonDeterministicFiniteAutomata(regex);
    auto new_regex = automata.get_regex();
    auto new_automata = NonDeterministicFiniteAutomata(new_regex);

    for (const auto& string : correct) {
      ASSERT_TRUE(new_automata.contains(string))
          << error_message(regex, string, CORRECT);
    }

    for (const auto& string : incorrect) {
      ASSERT_FALSE(new_automata.contains(string))
          << error_message(regex, string, INCORRECT);
    }
  }
}
