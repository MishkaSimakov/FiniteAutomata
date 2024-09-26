#include "DeterministicFiniteAutomata.h"

#include <queue>

#include "converters/NonDeterministicToDeterministic.h"

DeterministicFiniteAutomata::DeterministicFiniteAutomata() {
  nodes.emplace_back();

  for (size_t& jump : nodes.front().jumps) {
    jump = 0;
  }
}

DeterministicFiniteAutomata::DeterministicFiniteAutomata(
    const FiniteAutomata& finite_automata)
    : nodes(Converters::NonDeterministicToDeterministic()
                .convert(finite_automata)
                .nodes) {}

bool DeterministicFiniteAutomata::contains(std::string_view word) {
  size_t state = 0;
  for (char symbol : word) {
    state = nodes[state].jumps[Charset::get_index(symbol)];
  }

  return nodes[state].is_final;
}
