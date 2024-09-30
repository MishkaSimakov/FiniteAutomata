#include "FiniteAutomata.h"

#include <queue>

FiniteAutomata::FiniteAutomata() {
  nodes.emplace_back();

  for (size_t& jump : nodes.front().jumps) {
    jump = 0;
  }
}

void FiniteAutomata::optimize() {
  // find and remove unreachable nodes
  std::vector<bool> visited(nodes.size(), false);
  visited[0] = true;

  std::queue<size_t> queue;
  queue.push(0);

  while (!queue.empty()) {
    size_t current = queue.front();
    queue.pop();

    for (size_t next : nodes[current].jumps) {
      if (!visited[next]) {
        visited[next] = true;
        queue.push(next);
      }
    }
  }

  std::vector<size_t> new_indices(nodes.size());
  size_t index = 0;
  for (size_t i = 0; i < nodes.size(); ++i) {
    if (visited[i]) {
      new_indices[i] = index;
      nodes[index] = std::move(nodes[i]);
      ++index;
    }
  }

  nodes.resize(index);

  for (Node& node : nodes) {
    for (size_t& jump : node.jumps) {
      jump = new_indices[jump];
    }
  }
}
