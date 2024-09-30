#include "FiniteAutomata.h"

#include <queue>

FiniteAutomata::FiniteAutomata(
    const NonDeterministicFiniteAutomata& automata) {
  using NDNode = typename NonDeterministicFiniteAutomata::Node;
  using NDNodeCref = std::reference_wrapper<const NDNode>;
  using DNode = FiniteAutomata::Node;

  const auto& old_nodes_list = automata.get_nodes();
  std::vector<NDNodeCref> old_nodes(old_nodes_list.begin(),
                                    old_nodes_list.end());

  std::unordered_set<const NDNode*> start_nodes;
  start_nodes.insert(&automata.get_nodes().front());
  NonDeterministicFiniteAutomata::do_empty_jumps(start_nodes);

  // add new start node
  DNode& start = nodes.emplace_back();

  std::vector start_state(automata.size(), false);
  for (size_t i = 0; i < start_state.size(); ++i) {
    const NDNode* current_node = &old_nodes[i].get();
    if (start_nodes.contains(current_node)) {
      start_state[i] = true;
      start.old_indices.insert(i);

      if (current_node->is_final) {
        start.is_final = true;
      }
    }
  }

  std::unordered_map<std::vector<bool>, size_t> old_to_new;
  old_to_new.emplace(start_state, 0);

  std::queue<std::vector<bool>> queue;
  queue.emplace(start_state);

  while (!queue.empty()) {
    std::vector<bool> current_nodes = queue.front();
    queue.pop();

    DNode& current_new_node = nodes[old_to_new.at(current_nodes)];

    std::unordered_set<const NDNode*> current_nodes_pointers;
    for (size_t i = 0; i < current_nodes.size(); ++i) {
      if (current_nodes[i]) {
        current_nodes_pointers.insert(&old_nodes[i].get());
      }
    }

    for (auto symbol : Charset::get_symbols()) {
      auto copy = current_nodes_pointers;
      NonDeterministicFiniteAutomata::do_jumps(copy, symbol);

      bool is_final = false;
      std::vector mask_after_jumps(automata.size(), false);
      for (size_t i = 0; i < automata.size(); ++i) {
        if (copy.contains(&old_nodes[i].get())) {
          mask_after_jumps[i] = true;

          if (old_nodes[i].get().is_final) {
            is_final = true;
          }
        }
      }

      auto [itr, was_emplaced] =
          old_to_new.emplace(mask_after_jumps, nodes.size());

      if (was_emplaced) {
        current_new_node.jumps[Charset::get_index(symbol)] = nodes.size();
        DNode& new_node = nodes.emplace_back();
        new_node.is_final = is_final;

        for (size_t i = 0; i < mask_after_jumps.size(); ++i) {
          if (mask_after_jumps[i]) {
            new_node.old_indices.insert(i);
          }
        }

        queue.push(mask_after_jumps);
        continue;
      }

      size_t jump_index = itr->second;
      current_new_node.jumps[Charset::get_index(symbol)] = jump_index;
    }
  }
}
