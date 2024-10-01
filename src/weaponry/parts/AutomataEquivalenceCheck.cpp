#include "FiniteAutomata.h"

namespace {
// It would be better to use recursive lambda here. But alas! apple clang
// doesn't support recursive lambdas
struct IsomorphismChecker {
  using Node = FiniteAutomata::Node;

  const std::deque<Node>& left_nodes;
  const std::deque<Node>& right_nodes;

  std::vector<int> isomorphism;

  IsomorphismChecker(const std::deque<Node>& left_nodes,
                     const std::deque<Node>& right_nodes)
      : left_nodes(left_nodes),
        right_nodes(right_nodes),
        isomorphism(left_nodes.size(), -1) {}

  bool is_isomorphic_recursive(size_t left_index, size_t right_index) {
    if (isomorphism[left_index] != -1) {
      return isomorphism[left_index] == right_index;
    }

    isomorphism[left_index] = static_cast<int>(right_index);

    const Node& left = left_nodes[left_index];
    const Node& right = right_nodes[right_index];

    if (left.is_final != right.is_final) {
      return false;
    }

    for (size_t symbol = 0; symbol < Charset::symbols_count; ++symbol) {
      if (!is_isomorphic_recursive(left.jumps[symbol], right.jumps[symbol])) {
        return false;
      }
    }

    return true;
  }
};
}  // namespace

bool FiniteAutomata::is_isomorphic_to(const FiniteAutomata& other) const {
  // compare if automatas are isomorphic
  if (nodes.size() != other.nodes.size()) {
    return false;
  }

  IsomorphismChecker checker(nodes, other.nodes);
  return checker.is_isomorphic_recursive(0, 0);
}

bool FiniteAutomata::is_language_equal(const FiniteAutomata& other) const {
  auto left_minimal = get_minimal();
  auto right_minimal = other.get_minimal();

  return left_minimal.is_isomorphic_to(right_minimal);
}
