#include "FiniteAutomata.h"

#include <algorithm>
#include <queue>

#include "CharsetInfo.h"

template class FiniteAutomata<CharsetInfo>;
template class FiniteAutomata<BinaryCharsetInfo>;

template<typename Charset>
void FiniteAutomata<Charset>::remove_unreachable_nodes() {
  std::unordered_set<const Node*> visited;
  std::queue<const Node*> queue;

  visited.insert(&nodes_.front());
  queue.push(&nodes_.front());

  while (!queue.empty()) {
    const Node* current = queue.front();
    queue.pop();

    for (const Node* next : current->jumps | std::views::values) {
      auto [itr, was_inserted] = visited.insert(next);

      if (was_inserted) {
        queue.push(next);
      }
    }
  }

  for (auto itr = nodes_.begin(); itr != nodes_.end();) {
    if (visited.contains(&*itr)) {
      ++itr;
    } else {
      itr = nodes_.erase(itr);
    }
  }
}

template<typename Charset>
void FiniteAutomata<Charset>::remove_empty_jumps() {
  for (auto& node : nodes_) {
    std::unordered_set<Node*> nodes;
    nodes.insert(&node);

    do_empty_jumps(nodes);

    // add appropriate jumps
    std::vector<std::pair<char, Node*>> additional_jumps;
    for (const Node* reachable_by_empty_jump : nodes) {
      if (reachable_by_empty_jump->is_final) {
        node.is_final = true;
      }

      for (const auto& [symbol, dest] : reachable_by_empty_jump->jumps) {
        if (symbol != cEmptyChar) {
          additional_jumps.emplace_back(symbol, dest);
        }
      }
    }

    for (auto& [symbol, dest] : additional_jumps) {
      node.jumps.emplace(symbol, dest);
    }

    // remove all empty jumps from node
    node.jumps.erase(cEmptyChar);
  }

  remove_unreachable_nodes();
}
