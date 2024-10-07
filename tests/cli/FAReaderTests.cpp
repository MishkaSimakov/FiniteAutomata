#include <gtest/gtest.h>

#include "FiniteAutomata.h"
#include "FiniteAutomataPrinter.h"
#include "RegexStrings.h"
#include "cli/FiniteAutomataReader.h"
#include "cli/NonDeterministicFiniteAutomataReader.h"

TEST(FAReaderTests, test_it_reads_ndfa_correctly) {
  std::stringstream ss;

  ss << "n a0 b1 -2\n";
  ss << "n b1 b0\n";
  ss << "f\n";
  ss << "e";

  auto automata = NonDeterministicFiniteAutomataReader().read(ss);

  ASSERT_EQ(automata.get_nodes().size(), 3);

  size_t final_count = 0;
  for (const auto& node : automata.get_nodes()) {
    if (node.is_final) {
      ASSERT_TRUE(node.jumps.empty());

      ++final_count;
    }
  }

  ASSERT_EQ(final_count, 1);

  const auto& start_jumps = automata.get_start_node().jumps;
  ASSERT_TRUE(start_jumps.size() == 3 && start_jumps.contains('a') &&
              start_jumps.contains('b') &&
              start_jumps.contains(NonDeterministicFiniteAutomata::cEmptyChar));
  ASSERT_TRUE(start_jumps.find('a')->second == &automata.get_start_node());

  auto& q1 = *start_jumps.find('b')->second;
  ASSERT_FALSE(q1.is_final);

  ASSERT_TRUE(q1.jumps.size() == 2);
  ASSERT_TRUE(q1.jumps.contains('b'));
}

TEST(FAReaderTests, test_it_reads_dfa_correctly) {
  std::stringstream ss;

  ss << "f 0 1\n";
  ss << "f 1 3\n";
  ss << "f 1 3\n";
  ss << "n 1 3\n";
  ss << "n 4 1\n";
  ss << "e";

  auto automata = FiniteAutomataReader().read(ss);

  auto expected = FiniteAutomata();
  expected.nodes.resize(5);

  auto& q0 = expected.nodes[0];
  auto& q1 = expected.nodes[1];
  auto& q2 = expected.nodes[2];
  auto& q3 = expected.nodes[3];
  auto& q4 = expected.nodes[4];

  q0.is_final = true;
  q1.is_final = true;
  q2.is_final = true;

  q0.jumps = {0, 1};
  q1.jumps = {1, 3};
  q2.jumps = {1, 3};
  q3.jumps = {1, 3};
  q4.jumps = {4, 1};

  ASSERT_TRUE(expected.is_isomorphic_to(automata));
}