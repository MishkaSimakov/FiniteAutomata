#include <gtest/gtest.h>

#include "FiniteAutomata.h"
#include "FiniteAutomataPrinter.h"
#include "RegexStrings.h"

TEST(NonDeterministicToDeterministicTests, test_it_draws_all_edges) {
  for (const Regex& regex : regex_strings | std::views::elements<0>) {
    auto automata = FiniteAutomata(regex);

    std::stringstream os;
    os << FiniteAutomataPrinter(automata);

    std::string result = os.str();

    for (size_t i = 0; i < automata.nodes.size(); ++i) {
      const auto& node = automata.nodes[i];

      for (size_t j = 0; j < node.jumps.size(); ++j) {
        std::string edge =
            fmt::format("{} -> {} [label = \"{}\"];", i, node.jumps[j],
                        Charset::get_symbols()[j]);

        ASSERT_TRUE(result.contains(edge));
      }
    }
  }
}
