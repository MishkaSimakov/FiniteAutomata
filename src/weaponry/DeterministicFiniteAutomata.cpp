#include "DeterministicFiniteAutomata.h"

#include <queue>

DeterministicFiniteAutomata::DeterministicFiniteAutomata(
    const FiniteAutomata& finite_automata) {
  using PairT = std::pair<const std::vector<bool>, Node*>;

  std::vector<Node> other_nodes(finite_automata.get_nodes().begin(), finite_automata.get_nodes().end());

  std::vector current_states(finite_automata.size(), false);
  current_states[0] = true;

  Node& start = nodes_.emplace_back();
  nodes.emplace(std::move(current_states), &start);

  std::queue<std::reference_wrapper<PairT>> queue;
  queue.emplace(*nodes.begin());

  while (!queue.empty()) {
    auto& [current_states, node_ptr] = queue.front().get();


  }
}
